all: abstract_memory.h cache.c cache.h cachesim.c common.c common.h direct_cache.c direct_cache.h full_cache.c full_cache.h memory.c memory.h parse_config.c parse_config.h random.c random.h statistics.c statistics.h trace.c trace.h
	gcc -o cachesim cache.c cachesim.c common.c direct_cache.c full_cache.c memory.c parse_config.c random.c statistics.c trace.c -m32 -std=gnu99 -g -gstabs

clean:
	-rm cachesim
