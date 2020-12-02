#authors: Janik und Len
#!/bin/bash

fh_test_status()
{
    case $1 in
        0) 
            echo -e  "\033[32mPassed\033[0m"
        ;;
        *)
            echo -e "\033[31mFailed\033[0m"
            echo
        ;;
    esac
}


fh_test_run()
{
    call=$1
    ${call} >out.teststdin 2>out.teststderr
    status=$?
    printf "%-65.65s  %-s\n" "$call" `fh_test_status $status`

    if [ $status -ne 0 ]; then
        cat out.teststdin
        cat out.teststderr
        echo
    fi
}

fh_test_run_equals() 
{
    call=$1
    equal=$2
    ${call} >out.teststdin 2>out.teststderr
    status=$?
    calling=${call}

    if [ '$calling'='$equal' ]; then
        printf "%-65.65s  %-s\n" "$call" `fh_test_status $status`
    else    
        status=1
    fi    
    if [ $status -ne 0 ]; then
        printf "%-60.60s  %-s\n" "$call" `fh_test_status $status`
        echo -e "\033[33mExpected: \033[0m"$Diff
        cat out.teststdin
        cat out.teststderr
        echo
    fi
}

fh_test_run_diff() 
{
    call=$1
    resPos=$2
    expectedPos=$3
    ${call} >out.teststdin 2>out.teststderr
    status=$?
    Diff=$(diff -w $resPos $expectedPos)
    
    if [ "$Diff" != "" ] || [ $status -ne 0 ]; then
        status=1
        printf "%-65.65s  %-s\n" "$call" `fh_test_status $status`
        echo -e "\033[33mExpected: \033[0m"$Diff
        cat out.teststdin
        cat out.teststderr
        echo
    else
        printf "%-65.65s  %-s\n" "$call" `fh_test_status $status`
    fi
}

