echo "What's the args?"

if [ "$#" -ne 1 ]
then
    echo "Run with one argument."
    exit -1
fi

echo "$# arguments: $0 and $1"
