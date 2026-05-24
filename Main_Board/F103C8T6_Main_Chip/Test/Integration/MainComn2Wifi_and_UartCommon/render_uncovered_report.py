#!/usr/bin/env python3
"""Render a readable lcov-like report from gcovr JSON.

The report intentionally keeps lcov-style markers:
  SF   - source file
  LF   - executable lines found
  LH   - executable lines hit
  BRF  - branches found
  BRH  - branches hit
  DA   - uncovered source line, with source code appended
  BRDA - uncovered branch, with source code appended
"""

import argparse
import json
from pathlib import Path


def parse_args(void=None):
    parser = argparse.ArgumentParser()
    parser.add_argument("--json", required=True, help="gcovr JSON report path")
    parser.add_argument("--root", required=True, help="repository root path")
    parser.add_argument("--output", required=True, help="output report path")
    return parser.parse_args(void)


def normalize_path(path):
    return path.replace("\\", "/")


def resolve_source_path(root, reported_file):
    source_path = Path(reported_file)

    if source_path.is_absolute():
        return source_path

    candidate = root / reported_file
    if candidate.exists():
        return candidate

    normalized_reported = normalize_path(reported_file)
    for path in root.rglob(Path(reported_file).name):
        if normalize_path(str(path)).endswith(normalized_reported):
            return path

    return candidate


def read_source_lines(source_path):
    try:
        return source_path.read_text(encoding="utf-8", errors="replace").splitlines()
    except OSError:
        return []


def source_text(source_lines, line_number):
    index = line_number - 1
    if 0 <= index < len(source_lines):
        return source_lines[index].rstrip()
    return "<source line unavailable>"


def display_source_path(root, source_path, reported_file):
    try:
        return normalize_path(str(source_path.relative_to(root)))
    except ValueError:
        reported = normalize_path(reported_file)
        root_text = normalize_path(str(root))
        if reported.startswith(root_text + "/"):
            return reported[len(root_text) + 1:]
        return Path(reported).name if Path(reported).is_absolute() else reported


def render_file(file_entry, root):
    reported_file = file_entry.get("file", file_entry.get("filename", "?"))
    source_path = resolve_source_path(root, reported_file)
    source_lines = read_source_lines(source_path)

    rel_path = display_source_path(root, source_path, reported_file)
    executable_lines = file_entry.get("lines", [])
    branches = [
        (line.get("line_number", 0), branch_index, branch)
        for line in executable_lines
        for branch_index, branch in enumerate(line.get("branches", []))
    ]

    uncovered_lines = [
        line for line in executable_lines
        if line.get("count", 0) == 0
    ]
    uncovered_branches = [
        (line_number, branch_index, branch)
        for line_number, branch_index, branch in branches
        if branch.get("count", 0) == 0
    ]

    lines_found = len(executable_lines)
    lines_hit = sum(1 for line in executable_lines if line.get("count", 0) > 0)
    branches_found = len(branches)
    branches_hit = sum(1 for _, _, branch in branches if branch.get("count", 0) > 0)

    output = [
        f"SF:{rel_path}",
        f"LF:{lines_found}",
        f"LH:{lines_hit}",
        f"BRF:{branches_found}",
        f"BRH:{branches_hit}",
    ]

    if uncovered_lines:
        output.append("UNCOVERED_LINES:")
        for line in uncovered_lines:
            line_number = line.get("line_number", 0)
            output.append(f"DA:{line_number},0 | {source_text(source_lines, line_number)}")
    else:
        output.append("UNCOVERED_LINES:none")

    if uncovered_branches:
        output.append("UNCOVERED_BRANCHES:")
        for line_number, branch_index, branch in uncovered_branches:
            count = branch.get("count", 0)
            output.append(f"BRDA:{line_number},0,{branch_index},{count} | {source_text(source_lines, line_number)}")
    else:
        output.append("UNCOVERED_BRANCHES:none")

    output.append("end_of_record")
    return "\n".join(output)


def main():
    args = parse_args()
    root = Path(args.root).resolve()
    json_path = Path(args.json)
    output_path = Path(args.output)

    report = json.loads(json_path.read_text(encoding="utf-8"))
    files = report.get("files", [])

    rendered = [
        "LCOV-like uncovered report",
        "NOTE: DA and BRDA entries include source code after '|'.",
        "",
    ]
    rendered.extend(render_file(file_entry, root) for file_entry in files)

    output_path.write_text("\n\n".join(rendered) + "\n", encoding="utf-8")


if __name__ == "__main__":
    main()
