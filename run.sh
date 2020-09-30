./a.out < "Tests/test${1}.in" > "GUI/test${1}.in"
python3 GUI/plot.py < "GUI/test${1}.in"