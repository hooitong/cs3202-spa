echo off
cls
echo "Start Test"
start "" ..\..\EmptyGeneralTesting\Debug\AutoTester.exe Advanced_Source.txt QueryMerged.txt QueryMerged.xml

echo "Done: Check Result at QueryMerged.xml"