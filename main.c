#include <stdlib.h>
#include <stdio.h>

unsigned sp = 0;
char *src = "(-((4)*((5+15)/2)))";

void next();
void
next()
{
	if(src[sp] == '\0') return;
	if(src[++sp] == '\0'){
		printf("note: eof reached\n");
	}
}

char expect(char c);
char
expect(char c)
{
	if(src[sp] != c){
		return 1;
	}
	next();
	return 0;
}

char parse_root(int *val);
char parse_expr(int *val);
char parse_paren(int *val);
char parse_sum(int *val);
char parse_mul(int *val);
char parse_term(int *val);
char parse_number(int *val);
char parse_nnumber(int *val);

char
parse_root(int *val)
{
	return parse_expr(val);
}

char
parse_expr(int *val)
{
	char rc;
	
	if(!parse_sum(val)) return 0;
	
	return 1;
}

/* note: there are 2 types of parsing issues:
 	 1) invalid path, try another
    	 2) path incorrect midway, crash with error
*/
char
parse_paren(int *val)
{
	char rc;
	if(expect('(')) return 1;
	rc = parse_expr(val);
	if(expect(')')){
	printf("%s\n", src + sp);
		printf("error: expected )");
		exit(1);
	}
	return rc;
}

char
parse_sum(int *val)
{
	int r1, r2;
	if(parse_mul(&r1)) return 1;
	if(!expect('+')){
		if(parse_sum(&r2)){
			printf("error: expected sum");
			exit(1);
		}
		*val = r1 + r2;
		return 0;
	}
	if(!expect('-')){
		if(parse_sum(&r2)){
			printf("error: expected sum");
			exit(1);
		}
		*val = r1 - r2;
		return 0;
	}
	*val = r1;
	return 0;
}

char
parse_mul(int *val)
{
	int r1, r2;
	if(parse_term(&r1)) return 1;
	if(!expect('*')){
		if(parse_mul(&r2)){
			printf("error: expected mul");
			exit(1);
		}
		*val = r1 * r2;
		return 0;
	}
	if(!expect('/')){
		if(parse_mul(&r2)){
			printf("error: expected mul");
			exit(1);
		}
		*val = r1 / r2;
		return 0;
	}
	*val = r1;
	return 0;
}

char
parse_term(int *val)
{
	if(!parse_number(val)) return 0;
	if(!parse_nnumber(val)) return 0;
	if(!parse_paren(val)) return 0;
	
	return 1;
}

char
parse_number(int *val)
{
	if(src[sp] < '0' || src[sp] > '9') return 1;
	unsigned r = 0, fact = 1, bsp;
	int count = 0;
	while(src[sp] >= '0' && src[sp] <= '9'){
		count++;
		next();
	}
	bsp = sp--;
	while(count > 0){
		r += fact * (src[sp] - '0');
		count--;
		sp--;
		fact *= 10;
	}
	sp = bsp;
	*val = r;
	return 0;
}

char
parse_nnumber(int *val)
{
	if(expect('-')) return 1;
	int r;
	if(parse_term(&r)){
		printf("error: expected term\n");
		exit(1);
	}
	*val = -r;
	return 0;
}

int
main()
{
	int r;
	parse_root(&r);
	printf("%d\n", r);
}
