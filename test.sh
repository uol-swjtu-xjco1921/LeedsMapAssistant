run_test () 
    { # run_test()
    # capture returned message
    message=$($1 $2) 
    # run again (pipe to null so it doesn't display to user) for the output code
    echo "Test case: $1 $2" 
    $1 $2 > null 
    out=$?
    # increment total by 2 because 2 tests are run (code and message)
    total=$(( $total+2 ))

    # check output value against expected
    if [ $out == $3 ]
    then
        echo "PASSED [return value]: expected output value \"$3\" got $out"
        # if we passed, increment the score
        (( score++ ))
    else
        echo "FAILED [return value]: expected output value \"$3\" got $out"
    fi

    # check output message against expected
    if [ "$message" == "$4" ] 
    then
        echo "PASSED [output message]: expected printout message \"$4\" got \"$message\""
        (( score++ ))
    else
        echo "FAILED [output message]: expected printout message \"$4\" got \"$message\""
    fi

    } # end run_test()

path="./testFile/"
testExecutable=main


filename="bad1.map"
full_path=$path$filename
run_test ./$testExecutable "" 1 "Bad argument count"

filename="bad2.map"
full_path=$path$filename
run_test ./$testExecutable $full_path 2 "ERROR: Errors exist in opening map file"

filename="bad3.map"
full_path=$path$filename
run_test ./$testExecutable $full_path 3 "ERROR: NodeNum=0"

filename="bad4.map"
full_path=$path$filename
run_test ./$testExecutable $full_path 4 "ERROR: LinkNum=0"

filename="bad5.map"
full_path=$path$filename
run_test ./$testExecutable $full_path 5 "ERROR: Bounding not found"
