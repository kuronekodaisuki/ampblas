/*----------------------------------------------------------------------------
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
 * asum.h
 *
 *---------------------------------------------------------------------------*/

#include "../ampblas_config.h"

namespace ampblas {
namespace _detail {

//
// asum_helper
//   Functor for ASUM reduction 
//

template<typename ret_type, typename value_type, typename x_type, typename functor>
struct asum_helper
{
    asum_helper(const value_type& value, const functor& sum_op) restrict(cpu, amp)
        : init_value(value), op(sum_op) 
    {
    }

    // computes the sum of lhs and the absolute value of X[idx] and stores results in lhs
    void local_reduce(value_type& lhs, int idx, const x_type& X) const restrict(cpu, amp)
    {
        lhs += abs(X[concurrency::index<1>(idx)]);
    }

    // reduction of container vec
    ret_type global_reduce(const std::vector<value_type>& vec) const
    {
         return std::accumulate(vec.begin(), vec.end(), init_value);
    }

    value_type init_value;
    functor op;
};

} // namespace _detail

//-------------------------------------------------------------------------
// ASUM
//  computes the sum of the absolute values in a container.
//-------------------------------------------------------------------------

template <typename x_type>
typename real_type<typename x_type::value_type>::type asum(int n, const x_type& X)
{
    typedef typename x_type::value_type T;
    typedef typename real_type<T>::type real_type;

    // tuning parameters
    static const unsigned int tile_size = 128;
    static const unsigned int max_tiles = 64;

    auto func = _detail::asum_helper<real_type, real_type, x_type, _detail::sum<real_type>>(real_type(), _detail::sum<real_type>());

    // call generic 1D reduction
    return _detail::reduce<tile_size, max_tiles, real_type, real_type>(n, X, func);
}

template <typename value_type>
typename real_type<value_type>::type asum(const int n, const value_type* x, const int incx)
{
    typedef typename real_type<value_type>::type real_type;

    // quick return
	if (n == 0 || incx <= 0)
		return real_type();

	// argument check
	if (x == nullptr)
		argument_error("asum", 2);

    auto x_vec = make_vector_view(n, x, incx);

    return asum(n, x_vec);
}

} // namespace ampblas