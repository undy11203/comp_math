EXECUTABLE="$1"

TOTAL_TESTS=0
SUCCESSFUL_TESTS=0

TEST_CASES=(
    "0.00001 103.27 326.901 -9.9"
    "0.00001 10664.99 -1076606.65 10765"
    "0.00001 -103.33 333.099 -9.9"
    "0.00001 10010 100025 250000"
    "0.00001 3319 -46613 163317"
    "0.00001 -10010 100025 -250000"
    "0.00001 -99997.78 -221998.7679 -123210"
    "0.00001 -111000 0 0"
    "0.00001 17.6 77.44 0"
    "0.00001 -17.6 77.44 0"
    "0.00001 29.7 294.03 970.299"
    "0.00001 -21.3 151.23 -357.911"
    "0.00001 0 12345 0"
    "0.00001 -1.12 5 -5.6"
    "0.00001 1.12 5 5.6"
    "0.00001 0 0 0"
    "0.0000001 -0.000001 1 -0.000001"
    "0.0001 -0.000001 1 -0.000001"
    "0.1 -0.000001 1 -0.000001"
    "1 -0.000001 1 -0.000001"
    "0.00000001 0 -2 0"
    "0.0001 0 2 0"
    "0.1 0 -2 0"
    "1 0 -2 0"
)

run_test() {
    IFS=" " read -r EPSILON A B C <<< "$1"
    echo "Testing with epsilon=$EPSILON, a=$A, b=$B, c=$C"

    # Run the program with the specified parameters and capture the OUTPUT
    OUTPUT=$("$EXECUTABLE" <<< "$EPSILON $A $B $C")

    # Initialize a flag to check if all results satisfy the equation within epsilon
    ALL_ROOTS_OK=true

    # Search for lines with results in the program's OUTPUT and display them
    while IFS= read -r OUTPUT_LINE; do
        if [[ $OUTPUT_LINE == "Root:"* ]]; then

            # Extract the found ROOT from the line
            ROOT=$(echo "$OUTPUT_LINE" | cut -d' ' -f2)

            # Evaluate the equation with the ROOT within the script
            EQUATION_RESULT=$(bc -l <<< "$ROOT^3 + $A*$ROOT^2 + $B*$ROOT + $C")
            ABS_RESULT=$(echo "$EQUATION_RESULT" | tr -d '-')

            # Check if the absolute value of the equation result is less than epsilon
            if (( $(echo "$ABS_RESULT < $EPSILON" | bc -l) )); then
                echo "  $OUTPUT_LINE OK."
            else
                echo "  $OUTPUT_LINE NO OK."
                ALL_ROOTS_OK=false
            fi
        fi
    done <<< "$OUTPUT"

    if $ALL_ROOTS_OK ; then
        ((SUCCESSFUL_TESTS++))
    fi

    ((TOTAL_TESTS++))
    echo "---------------------------------------------------------"
}


# Run tests for all sets of parameters
for TEST_CASE in "${TEST_CASES[@]}"; do
    run_test "$TEST_CASE"
done

# Display the total number of tests and successful tests
echo "Total tests: $TOTAL_TESTS"
echo "Successful tests: $SUCCESSFUL_TESTS"


