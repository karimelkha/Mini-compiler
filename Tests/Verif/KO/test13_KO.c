// Error line 6: variable of type TYPE INT is initialized with an expression of type TYPE BOOL.
int start = 0;
int end = 100;

void main(){
	bool a = 1;
	int i , s = start, e = end;
	int sum = 0;
	for (i = s; i < e; i = i + 1){
		sum = sum + i;
	}
	print("sum: ", sum, "\n");
}