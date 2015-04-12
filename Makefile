all:
	g++ -std=c++11 src/main.cpp -o build/output
install:
	cp build/output /usr/bin/ft
github:
	if [ ! -z "$(ls -l | grep build)" ]; then git rm -r build; fi
	git add src Makefile README.md
	git commit -m "Updated source"
	git push -u origin master
