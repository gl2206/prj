#include <stdio.h>
#include <gmp.h>

void euclide(mpz_t res, mpz_t a, mpz_t b) {
	mpz_t r, u, v, r_, u_, v_, r_temp, u_temp, v_temp, q;
	mpz_init_set(r, a);
	mpz_init_set(r_, b);
	mpz_init_set_ui(u, 1);
	mpz_init_set_ui(v, 0);
	mpz_init_set_ui(u_, 0);
	mpz_init_set_ui(v_, 1);
	mpz_init(r_temp);
	mpz_init(u_temp);
	mpz_init(v_temp);
	mpz_init(q);
	
	while(mpz_cmp_ui(r_, 0)) {
		mpz_fdiv_q(q, r, r_);
		mpz_set(r_temp, r);
		mpz_set(u_temp, u);
		mpz_set(v_temp, v);
		mpz_set(r, r_);
		mpz_set(u, u_);
		mpz_set(v, v_);
		mpz_submul(r_temp, q, r_);
		mpz_set(r_, r_temp);
		mpz_submul(u_temp, q, u_);
		mpz_set(u_, u_temp);
		mpz_submul(v_temp, q, v_);
		mpz_set(v_, v_temp);
	}
	mpz_set(res, v);
}

void chiffrement(mpz_t res, mpz_t msg, mpz_t n, mpz_t e) {
	mpz_powm(res, msg, e, n);
}

void dechiffrement(mpz_t res, mpz_t msg, mpz_t n, mpz_t d) {
	mpz_powm(res, msg, d, n);
}

int main() {
	mpz_t p, q, n, phi, e, d, msg;
	
	mpz_init_set_str(p, "3", 10);
	mpz_init_set_str(q, "11", 10);
	mpz_init(n);
	mpz_init(phi);
	
	mpz_mul(n, p, q);
	
	gmp_printf("p=%Zd\nq=%Zd\nn=p*q=%Zd\n", p, q, n);
	
	mpz_sub_ui(p, p, 1);
	mpz_sub_ui(q, q, 1);
	mpz_mul(phi, p, q);
	
	gmp_printf("phi=(p-1)*(q-1)=%Zd\n", phi);
	
	mpz_init_set_str(e, "3", 10);
	
	mpz_init(d);
	euclide(d, phi, e);
	
	gmp_printf("e=%Zd\nd=%Zd\n", e, d);
	
	mpz_init_set_str(msg, "4", 10);
	gmp_printf("msg = %Zd\n", msg);
	
	chiffrement(msg, msg, n, e);
	gmp_printf("msg crypte = %Zd\n", msg);
	
	dechiffrement(msg, msg, n, d);
	gmp_printf("msg decrypte = %Zd\n", msg);
	
	return 0;
} 
