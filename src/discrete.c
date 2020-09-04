#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define REG_SIZE	32

int *bitarray(const int x, const int n_bits);
int gcd(int a, int b);
int *extended_euclid(int a, int b);
int modular_exp(int base, int power, int modulus);
int modular_inverse(int a, const int modulus);
int linear_congruence(int a, int b, const int modulus);
//int square_root(int base, int modulus);
//int test_primal(int p_candidate);

int *bitarray(const int x, const int n_bits)
{
	unsigned *bits = (unsigned*)malloc(sizeof(unsigned)*n_bits);
	for(int bit =0; bit<n_bits; bit++)
	{
		bits[bit]  = (x >> bit) & 1;
	}	
	return bits;
}	

int gcd(int a, int b) // GCD using recursive Eulers alg. a > b;
{
	if (a < b) { int r = a; a = b; b = r;} // Reorder
	if (b == 0) { return a;}

	//printf("%u\t%u\n", a, b);
	int quotient, remainder;
	quotient = (int) a/b;
	remainder = a - quotient*b;
	return gcd(b, remainder);
}

int modular_inverse(int a, const int modulus){
	int *ext_euc = extended_euclid(modulus, a);
	if (ext_euc[2] != 1){ return 0;} //no solution
	else {return ext_euc[1] % modulus;}
}

int lin_congruence(int a, int b, const int modulus)
{
	int d = gcd(a, modulus);
	if (d != b) {return 0;} // no solution
	int ap = a/d,
	    bp = b/d,
	    modp = modulus/d;
	int t = modular_inverse(modp, ap);
	int z = t*bp % modulus;
       return z;	
	    
}

int *extended_euclid(int a, int b) // For ax + by = gcd(a,b). Outputs x, y, gcd(a,b)
{
	int *sol = (int*)malloc(sizeof(int)*3);
	if (a < b) { int r = a; a = b; b = r;} // Reorder
	if (b == 0) 
	{ 
		sol[0] = 1;
		sol[1] = 0;
		sol[2] = a;
		return sol;
	}
	int quotient, rpp, spp, tpp;
	int r = a,
	    rp = b,
	    s = 1,
	    sp = 0,
	    t = 0,
	    tp = 1;
	while (rp != 0) 
	{
		quotient = (int) r/rp;
		rpp =  rp;
		spp =  sp;
		tpp =  tp; // No parallel assignment
		rp = r - rpp*quotient,
		   sp = s - spp*quotient,
		   tp = t - tpp*quotient,
		   r = rpp,
		   s = spp,
		   t = tpp;
	}
	sol[0] = s;
	sol[1] = t;
	sol[2] = r;
	return sol;
}

int modular_exp(const int base, const int exponent, const int modulus) // Using basic power doubling algoirthm
{
	int *power_bitstring = bitarray(exponent, REG_SIZE);
	unsigned int result = 1;
	unsigned int z = base % modulus;

	for (int k = 0; k<REG_SIZE; k++){
		if (power_bitstring[k] == 1) { result = (result * z) % modulus;}
		z = (z*z) % modulus;
	}
	return result;
}

void main(int argc, char *argv[]) 
{
	// Test of bit array
	int *bits;
	const int b_len = 7;
	bits = bitarray(13, b_len);
	printf("Bitstring:\t");
	for(int bit=b_len; bit >=0; bit--)
	{
		printf("%u", bits[bit]);
	}
	printf("\n");

	// Test of gcd 
	int a = 2*2*5*7*13*19,
	    b = 2*2*2*2*7*13*19*43;
	int ab_gcd = gcd(a, b); //Result:6916
	printf("GCD Results:\t%u\n", ab_gcd);

	// Test of Modular Exponentiation
	int exp_result = modular_exp(5, 26, 13);
	printf("Exponentiation:\t%u\n", exp_result); //Result:12 

	// Test of Extended Euclid
	int *ext_euc_res;
	ext_euc_res = extended_euclid(a, b);
	printf("Extended Euclid:\t%d\t%d\t%d\n a*x + b*y = %d*%d + %d*%d = %d\n", ext_euc_res[0], ext_euc_res[1], ext_euc_res[2], b, ext_euc_res[0], a, ext_euc_res[1], b*ext_euc_res[0] + a*ext_euc_res[1]);
}
