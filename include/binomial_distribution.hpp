#ifndef PSERNG_BINOMIAL_DISTRIBUTION_HPP_
#define PSERNG_BINOMIAL_DISTRIBUTION_HPP_

#include <algorithm>
#include <random>
#include <functional>
#include <numeric>
#include <cassert>
#include "categorical_distribution.hpp"

namespace pserng { 

//!  Binomial distribution 
/*!
  Produces random non-negative integer values according to a binomial distribution with given
  probability and number of trials. Equivalent distribution to std::binomial_distribution, but
  more efficient for small number of trials.
*/
template <class IntType = int>
class binomial_distribution {
public:
    
    
    //! Type of generated values. Alias of IntType
	typedef IntType result_type;


    //! Constructor
    /*!
      \param t number of trials
      \param probabilty of event 1 
    */
	explicit binomial_distribution(IntType t = 1, double p = 0.5) : 
        t_(t), 
        p_(p) 
    {
    }


    //! Generate random vector following binomial distribution
    /*!
      \param g an uniform random bit generator object
      \return next random vector from given distribution 
    */
	template<class G>
	result_type operator()(G& g) const { 
		result_type pos_trials = 0; // number of positive trials out of total t_
		const auto g_min = g.min();
        const auto g_max = g.max();
        for (IntType tt=0; tt<t_; ++tt) {
			const auto val_g =  g();
			
            if ((val_g-g_min) < p_ * (g_max-g_min))
                ++pos_trials;
		}   
		return pos_trials;
	}   


    //! Get number of trials
    /*!
      \return number of trials 
    */
	double p() const { return p_;};
    
    
    //! Get probability of event 1
    /*!
      \return probability of event 1 
    */
	result_type t() const {return t_;};

    //! Get minimum potentially generated value
    /*!
      \return minimum potentially generated value 
    */
	result_type min() const {return 0;};
	
    
    //! Get maximum potentially generated value
    /*!
      \return maximum potentially generated value 
    */
    result_type max() const {return t_;};

private:

    // number of trials
    IntType t_; 

    // probability of event 1
	double p_; 

};


} // namespace pserng

#endif//PSERNG_BINOMIAL_DISTRIBUTION_HPP_

