/* 
 * Copyright � Microsoft Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not 
 * use this file except in compliance with the License.  You may obtain a copy 
 * of the License at http://www.apache.org/licenses/LICENSE-2.0  
 * 
 * THIS CODE IS PROVIDED *AS IS* BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION ANY IMPLIED 
 * WARRANTIES OR CONDITIONS OF TITLE, FITNESS FOR A PARTICULAR PURPOSE, 
 * MERCHANTABLITY OR NON-INFRINGEMENT. 
 *
 * See the Apache Version 2.0 License for specific language governing 
 * permissions and limitations under the License.
 *---------------------------------------------------------------------------
 * 
 * BLAS levels 1,2,3 library implementation files.
 *
 * This file contains AMP CBLAS wrappers to AMP C++ BLAS implementaion. 
 *
 *---------------------------------------------------------------------------*/
#include <assert.h>
#include "ampcblas.h"
#include "ampblas.h"

typedef ampblas::complex<float>  fcomplex;
typedef ampblas::complex<double> dcomplex;

#define AMPBLAS_CHECKED_CALL(...) \
    { \
        ampblas_result re = AMPBLAS_OK; \
        try \
        { \
            (__VA_ARGS__);\
        } \
        catch (ampblas::ampblas_exception &e) \
        { \
            re = e.get_error_code(); \
        } \
        catch (concurrency::runtime_exception&) \
        { \
            re = AMPBLAS_AMP_RUNTIME_ERROR; \
        } \
        catch (std::bad_alloc&) \
        { \
            re = AMPBLAS_OUT_OF_MEMORY; \
        } \
        catch (...) \
        { \
            re = AMPBLAS_INTERNAL_ERROR; \
        } \
        ampblas_set_last_error(re); \
    }

extern "C" 
{

//
// AMP CBLAS AMAX implementation file. 
// 

int ampblas_isamax(const int N, const float  *X, const int incX)
{
    int ret = 0;
    AMPBLAS_CHECKED_CALL( ret = ampblas::amax<int>(N, X, incX) );
    return ret;
}

int ampblas_idamax(const int N, const double *X, const int incX)
{
    int ret = 0;
    AMPBLAS_CHECKED_CALL( ret = ampblas::amax<int>(N, X, incX) );
    return ret;
}

//
// AMP CBLAS ASUM implementation file. 
//

float ampblas_sasum(const int N, const float *X, const int incX)
{
    float ret = 0;
    AMPBLAS_CHECKED_CALL( ret = ampblas::asum(N, X, incX) );
    return ret;
}

double ampblas_dasum(const int N, const double *X, const int incX)
{
    double ret = 0;
    AMPBLAS_CHECKED_CALL( ret = ampblas::asum(N, X, incX) );
    return ret;
}

// 
// AMP CBLAS AXPY implementation file.
// 
void ampblas_saxpy(const int N, const float alpha, const float *X,
                              const int incX, float *Y, const int incY)
{
    AMPBLAS_CHECKED_CALL(ampblas::axpy(N, alpha, X, incX, Y, incY));
}

void ampblas_daxpy(const int N, const double alpha, const double *X,
                   const int incX, double *Y, const int incY)
{
	AMPBLAS_CHECKED_CALL(ampblas::axpy(N, alpha, X, incX, Y, incY));
}

void ampblas_caxpy(const int N, const void *alpha, const void *X,
                   const int incX, void *Y, const int incY)
{
	fcomplex falpha =*(fcomplex*)(alpha);
	AMPBLAS_CHECKED_CALL(ampblas::axpy(N, falpha, (fcomplex*)X, incX, (fcomplex*)Y, incY));
}

void ampblas_zaxpy(const int N, const void *alpha, const void *X,
                   const int incX, void *Y, const int incY)
{
    dcomplex dalpha =*(dcomplex*)(alpha);
	AMPBLAS_CHECKED_CALL(ampblas::axpy(N, dalpha, (dcomplex*)X, incX, (dcomplex*)Y, incY));
}

// 
// AMP CBLAS COPY implementation file.
// 
void ampblas_scopy(const int N, const float *X, const int incX, float *Y, const int incY)
{
    AMPBLAS_CHECKED_CALL(ampblas::copy(N, X, incX, Y, incY));
}

void ampblas_dcopy(const int N, const double *X, const int incX, double *Y, const int incY)
{
	AMPBLAS_CHECKED_CALL(ampblas::copy(N, X, incX, Y, incY));
}

void ampblas_ccopy(const int N, const void *X, const int incX, void *Y, const int incY)
{
	AMPBLAS_CHECKED_CALL(ampblas::copy(N, (fcomplex*)X, incX, (fcomplex*)Y, incY));
}

void ampblas_zcopy(const int N, const void *X, const int incX, void *Y, const int incY)
{
	AMPBLAS_CHECKED_CALL(ampblas::copy(N, (dcomplex*)X, incX, (dcomplex*)Y, incY));
}

//
// AMP CBLAS DOT implementation file.
//

// float ampblas_sdsdot(const int N, const float alpha, const float *X, const int incX, const float *Y, const int incY) 
// {
// }

double ampblas_dsdot(const int N, const float *X, const int incX, const float *Y, const int incY)
{
    double ret = 0;
	AMPBLAS_CHECKED_CALL( ret = ampblas::dot<float,double,ampblas::_detail::noop>(N,X,incX,Y,incY) );
    return ret;
}

float ampblas_sdot(const int N, const float  *X, const int incX, const float  *Y, const int incY)
{
    float ret = 0;
    AMPBLAS_CHECKED_CALL( ret = ampblas::dot<float,float,ampblas::_detail::noop>(N,X,incX,Y,incY) );
    return ret;
}

double ampblas_ddot(const int N, const double *X, const int incX, const double *Y, const int incY)
{   
    double ret = 0;
    AMPBLAS_CHECKED_CALL( ret = ampblas::dot<double,double,ampblas::_detail::noop>(N,X,incX,Y,incY) );
    return ret;
}

//
// AMP CBLAS GER implementation file
// 

void ampblas_sger(const enum AMPBLAS_ORDER order, const int M, const int N,
                  const float alpha, const float *X, const int incX,
                  const float *Y, const int incY, float *A, const int lda)
{
    AMPBLAS_CHECKED_CALL( ampblas::ger<float,ampblas::_detail::noop>(order,M,N,alpha,X,incX,Y,incY,A,lda) );
}

void ampblas_dger(const enum AMPBLAS_ORDER order, const int M, const int N,
                  const double alpha, const double *X, const int incX,
                  const double *Y, const int incY, double *A, const int lda)
{
	AMPBLAS_CHECKED_CALL( ampblas::ger<double,ampblas::_detail::noop>(order,M,N,alpha,X,incX,Y,incY,A,lda) );
}

//
// AMP CBLAS NRM2 implementation file.
//
float ampblas_snrm2(const int N, const float* X, int incX )
{
    float ret = 0;
    AMPBLAS_CHECKED_CALL(ret = ampblas::nrm2(N, X, incX));
    return ret;
}

double ampblas_dnrm2(const int N, const double* X, int incX )
{
    double ret = 0;
    AMPBLAS_CHECKED_CALL(ret = ampblas::nrm2(N, X, incX));
    return ret;
}

//
// AMP CBLAS ROT implementation file
// 

void ampblas_srot(const int N, float *X, const int incX, float *Y, const int incY, const float c, const float s)
{
    AMPBLAS_CHECKED_CALL(ampblas::rot<float>(N,X,incX,Y,incY,c,s));
}

void ampblas_drot(const int N, double *X, const int incX, double *Y, const int incY, const double c, const double s)
{    
    AMPBLAS_CHECKED_CALL(ampblas::rot<double>(N,X,incX,Y,incY,c,s));
}

//
// AMP CBLAS ROTG implementation file
// 

void ampblas_srotg(float *a, float *b, float *c, float *s)
{
    AMPBLAS_CHECKED_CALL( ampblas::rotg<float>(*a,*b,*c,*s) );
}

void ampblas_drotg(double *a, double *b, double *c, double *s)
{
    AMPBLAS_CHECKED_CALL( ampblas::rotg<double>(*a,*b,*c,*s) );
}

// 
// AMP CBLAS SCAL implementation file.
// 
void ampblas_sscal(const int N, const float alpha, float *X, const int incX)
{
    AMPBLAS_CHECKED_CALL(ampblas::scal(N, alpha, X, incX));
}

void ampblas_dscal(const int N, const double alpha, double *X, const int incX)
{
	AMPBLAS_CHECKED_CALL(ampblas::scal(N, alpha, X, incX));
}

void ampblas_cscal(const int N, const void *alpha, void *X, const int incX)
{
	const fcomplex falpha =*(fcomplex*)(alpha);
	AMPBLAS_CHECKED_CALL(ampblas::scal(N, falpha, (fcomplex*)X, incX));
}

void ampblas_zscal(const int N, const void *alpha, void *X, const int incX)
{
    dcomplex dalpha =*(dcomplex*)(alpha);
	AMPBLAS_CHECKED_CALL(ampblas::scal(N, dalpha, (dcomplex*)X, incX));
}

void ampblas_csscal(const int N, const float alpha, void *X, const int incX)
{
    AMPBLAS_CHECKED_CALL(ampblas::scal<fcomplex>(N, alpha, (fcomplex*)X, incX));
}

void ampblas_zdscal(const int N, const double alpha, void *X, const int incX)
{
	AMPBLAS_CHECKED_CALL(ampblas::scal<dcomplex>(N, alpha, (dcomplex*)X, incX));
}

//
// AMP CBLAS SYR implementation file
// 

void ampblas_ssyr(const enum AMPBLAS_ORDER order, const enum AMPBLAS_UPLO uplo, const int N,
                  const float alpha, const float *X, const int incX,
                  float *A, const int lda)
{
    AMPBLAS_CHECKED_CALL( ampblas::syr<float>(order,uplo,N,alpha,X,incX,A,lda) );
}

void ampblas_dsyr(const enum AMPBLAS_ORDER order, const enum AMPBLAS_UPLO uplo, const int N,
                  const double alpha, const double *X, const int incX,
                  double *A, const int lda)
{
	AMPBLAS_CHECKED_CALL( ampblas::syr<double>(order,uplo,N,alpha,X,incX,A,lda) );
}

// 
// AMP CBLAS SWAP implementation file.
// 
void ampblas_sswap(const int N, float *X, const int incX, float *Y, const int incY)
{
    AMPBLAS_CHECKED_CALL(ampblas::swap(N, X, incX, Y, incY));
}

void ampblas_dswap(const int N, double *X, const int incX, double *Y, const int incY)
{
	AMPBLAS_CHECKED_CALL(ampblas::swap(N, X, incX, Y, incY));
}

void ampblas_cswap(const int N, void *X, const int incX, void *Y, const int incY)
{
	AMPBLAS_CHECKED_CALL(ampblas::swap(N, (fcomplex*)X, incX, (fcomplex*)Y, incY));
}

void ampblas_zswap(const int N, void *X, const int incX, void *Y, const int incY)
{
	AMPBLAS_CHECKED_CALL(ampblas::swap(N, (dcomplex*)X, incX, (dcomplex*)Y, incY));
}

//
// AMP CBLAS GEMV implementation
//
void ampblas_sgemv(const enum AMPBLAS_ORDER order,
                               const enum AMPBLAS_TRANSPOSE TransA, const int M, const int N,
                               const float alpha, const float *A, const int lda,
                               const float *X, const int incX, const float beta,
                               float *Y, const int incY)
{
	AMPBLAS_CHECKED_CALL(ampblas::gemv<float>(order, TransA, M, N, alpha, A, lda, X, incX, beta, Y, incY));
}

void ampblas_dgemv(const enum AMPBLAS_ORDER order,
                               const enum AMPBLAS_TRANSPOSE TransA, const int M, const int N,
                               const double alpha, const double *A, const int lda,
                               const double *X, const int incX, const double beta,
                               double *Y, const int incY)
{
	AMPBLAS_CHECKED_CALL(ampblas::gemv<double>(order, TransA, M, N, alpha, A, lda, X, incX, beta, Y, incY));
}

// 
// AMP CBLAS GEMM implementation
// 

void ampblas_sgemm(const enum AMPBLAS_ORDER Order, const enum AMPBLAS_TRANSPOSE TransA,
                   const enum AMPBLAS_TRANSPOSE TransB, const int M, const int N,
                   const int K, const float alpha, const float *A,
                   const int lda, const float *B, const int ldb,
                   const float beta, float *C, const int ldc)
{
    AMPBLAS_CHECKED_CALL(ampblas::gemm(Order, TransA, TransB, M, N, K, alpha, A, lda, B, ldb, beta, C, ldc));
}

void ampblas_dgemm(const enum AMPBLAS_ORDER Order, const enum AMPBLAS_TRANSPOSE TransA,
                   const enum AMPBLAS_TRANSPOSE TransB, const int M, const int N,
                   const int K, const double alpha, const double *A,
                   const int lda, const double *B, const int ldb,
                   const double beta, double *C, const int ldc)
{
    AMPBLAS_CHECKED_CALL(ampblas::gemm(Order, TransA, TransB, M, N, K, alpha, A, lda, B, ldb, beta, C, ldc));
}

void ampblas_cgemm(const enum AMPBLAS_ORDER Order, const enum AMPBLAS_TRANSPOSE TransA,
                   const enum AMPBLAS_TRANSPOSE TransB, const int M, const int N,
                   const int K, const void *alpha, const void *A,
                   const int lda, const void *B, const int ldb,
                   const void *beta, void *C, const int ldc)
{
	fcomplex falpha =*(fcomplex*)(alpha);
	fcomplex fbeta  =*(fcomplex*)(beta);
    AMPBLAS_CHECKED_CALL(ampblas::gemm(Order, TransA, TransB, M, N, K, falpha, (fcomplex*)A, lda, (fcomplex*)B, ldb, fbeta, (fcomplex*)C, ldc));
}

void ampblas_zgemm(const enum AMPBLAS_ORDER Order, const enum AMPBLAS_TRANSPOSE TransA,
                   const enum AMPBLAS_TRANSPOSE TransB, const int M, const int N,
                   const int K, const void *alpha, const void *A,
                   const int lda, const void *B, const int ldb,
                   const void *beta, void *C, const int ldc)
{
	dcomplex dalpha =*(dcomplex*)(alpha);
	dcomplex dbeta  =*(dcomplex*)(beta);
    AMPBLAS_CHECKED_CALL(ampblas::gemm(Order, TransA, TransB, M, N, K, dalpha, (dcomplex*)A, lda, (dcomplex*)B, ldb, dbeta, (dcomplex*)C, ldc));
}

//
// AMP CBLAS TRSM implementation
// 

AMPBLAS_DLL void ampblas_strsm(const enum AMPBLAS_ORDER Order, const enum AMPBLAS_SIDE Side,
                               const enum AMPBLAS_UPLO Uplo, const enum AMPBLAS_TRANSPOSE TransA,
                               const enum AMPBLAS_DIAG Diag, const int M, const int N,
                               const float alpha, const float *A, const int lda,
                               float *B, const int ldb)
{
	AMPBLAS_CHECKED_CALL( ampblas::trsm(Order,Side,Uplo,TransA,Diag,M,N,alpha,A,lda,B,ldb) );
}

} // extern "C"