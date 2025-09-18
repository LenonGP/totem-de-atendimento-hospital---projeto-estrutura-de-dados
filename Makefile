CFLAGS=-Wpedantic -Wall -Wextra

all:totem tv

totem:menu.c tad_configs.c tad_configs.h ficha.c ficha.h
	cc $(CFLAGS) -o $@ menu.c tad_configs.c ficha.c

tv:simulacao.c tad_configs.c tad_configs.h ficha.c ficha.h arvore.h arvore.c
	cc $(CFLAGS) -o $@ simulacao.c tad_configs.c ficha.c arvore.c

clean:
	rm -f totem tv
