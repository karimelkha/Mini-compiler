// Manque une '}'
int start = 0;
int end = 100;
void main(){
	// Operation Arithmétique
	int temp = end;
	start = temp- temp/2;
	start = (start + 50 - 25)/75;
	start = start*temp - temp;


	int i, s = start, e = end;
	int sum = 0;
	for (i = s; i < e; i = i + 1){
		sum = sum + i;
	
	print("sum: ", sum, "\n");
}