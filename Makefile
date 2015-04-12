all:
	g++ -std=c++11 src/main.cpp -o build/output
install:
	cp build/output /usr/bin/ft
github:
	git rm -r build
	git add src Makefile README.md
	git commit -m "Updated source"
	git push -u origin master
