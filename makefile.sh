g++ --std=c++17 main.cpp -o MultipleBodyProblem.out
./MultipleBodyProblem.out
rm ./MultipleBodyProblem.out
/usr/bin/python3 ./data/dataAnalysis.py
rm ./data/positionData.txt
convert -delay 5 ./data/plotsForGif/*.png ./data/temp.gif
rm ./data/plotsForGif/*