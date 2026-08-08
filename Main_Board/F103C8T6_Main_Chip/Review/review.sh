git diff HEAD~1 > review.patch

mkdir -p cppcheck_logs

find ./../Systems ./../Test \
    \( -name "*.c" -o -name "*.h" \) \
    ! -ipath "*/mocks/*" \
    ! -ipath "*/stub/*" \
    ! -name "*TestRunner.c" \
    | while read file
do
    logname=$(echo "$file" | sed 's#[/.]#_#g')
    logfile="cppcheck_logs/${logname}.log"

    cppcheck \
        --enable=all \
        --quiet \
        --suppress=checkersReport \
        "$file" \
        2> "$logfile"

    if [ ! -s "$logfile" ]; then
        rm -f "$logfile"
    fi
done