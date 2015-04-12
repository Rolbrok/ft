all:
	g++ -std=c++11 src/main.cpp -o build/output
install:
	cp build/output /usr/bin/ft
github:
	git add Makefile src/*
	git commit -m "Updated source"
	git remote add origin git@github.com:Rolbrok/ft.git
	git push -u origin master
