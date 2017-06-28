#ifndef PSERNG_MULTINOMIAL_DISTRIBUTION_HPP_
#define PSERNG_MULTINOMIAL_DISTRIBUTION_HPP_

#include <algorithm>
#include <random>
#include <functional>
#include <numeric>
#include <cassert>
#include "binomial_distribution.hpp"

namespace pserng { 

//!  Multinomial distribution 
/*!
  Produces random vectors of non-negative integer values according to a 
  multinomial distribution with given probabilty for each category and given
  number of trials
*/
template <class IntType = int>
class multinomial_distribution {

public:
    
    
    //! Type of generated values. Alias of std::vector<IntType>
    using result_type = std::vector<IntType>;
   
    
    //! Constructor
    /*!
      \param t number of trials
      \param weights vector of probabilty weights for each class 
    */
    explicit multinomial_distribution(IntType t, std::vector<double> const& weights) : 
        t_(t),
        K_(weights.size()),
        binom_p_(K_)
    {
        // compute binomial probabilities
        double sum_weights = 0.;
        for (int k=0; k<K_; ++k)
            sum_weights += weights[k];

        double partsum_weights = 0;
        for (int k=0; k<K_; ++k) {
            binom_p_[k] = weights[k] / double(sum_weights - partsum_weights);
            partsum_weights += weights[k];
        }

    }


    //! Generate random vector following multinomial distribution
    /*!
      \param g an uniform random bit generator object
      \return next random vector from given distribution 
    */
    template <class G>
    result_type operator()(G& g) const {
        std::vector<IntType> counts(K_, 0);
        // iterate over each class and the counts of that class follows a binomial distribution.
        int t_avail = t_;
        for (IntType k=0; k<K_-1; ++k) {
            pserng::binomial_distribution<IntType> binom_dist(t_avail, binom_p_[k]);
            auto t_cnt = binom_dist(g);
            
            counts[k] = t_cnt;
            t_avail -= t_cnt;
        }
        counts[K_-1] = t_avail;

        return counts;
    
    }


    //! Get number of trials
    /*!
      \return number of trials 
    */
    inline IntType t() const { return t_; }



private:
    // number of categorical trials
    IntType t_;

    // number of classes
    IntType K_;

    // vector of binomial probabilities
    std::vector<double> binom_p_;

};


} // namespace pserng

#endif//PSERNG_MULTINOMIAL_DISTRIBUTION_HPP_
