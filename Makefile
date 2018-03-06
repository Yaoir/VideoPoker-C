videopoker: videopoker.c
	@gcc -std=c99 -pedantic -Wall -o videopoker videopoker.c

man: videopoker.6.ronn
	@ronn --roff --manual="Games Manual" --organization="Jay Ts" --date="2016-12-25" videopoker.6.ronn > man6/videopoker.6
	@gzip -f videopoker.6
	@mv videopoker.6.gz man6
	@man -l man6/videopoker.6.gz

showman:
	@man -l man6/videopoker.6.gz

# tran and wchar are test and development programs
# not included in the distribution

tran: tran.c
	@cc -Wall -o tran tran.c

wchar: wchar.c
	@gcc -std=c99 -pedantic -Wall -o wchar wchar.c

# install the manual page and program

install: installman
	@cp videopoker /usr/games

# install just the manual page

installman:
	@cp man6/videopoker.6.gz /usr/share/man/man6

backup:
	@cp *.c Makefile TODO *.ronn .bak

clean:
	@rm videopoker
