#ifndef PSERNG_CATEGORICAL_DISTRIBUTION_HPP_
#define PSERNG_CATEGORICAL_DISTRIBUTION_HPP_

#include <algorithm>
#include <random>
#include <functional>
#include <numeric>
#include <cassert>


namespace pserng { 

//!  Categorical distribution 
/*!
  Produces random non-negative integer values following a categorical
  distribution with given probabilty for each category
*/
template <class IntType = int>
class categorical_distribution {

public:


    //! Type of generated values. Alias of IntType
    using result_type = IntType;
   
    
    //! Constructor
    /*!
      \param weights vector of probabilty weights for each class 
    */
    explicit categorical_distribution(std::vector<double> const& weights) : 
        weights_(weights),
        partsums_weights_(weights_.size()) 
    {
        std::partial_sum(weights_.begin(), weights_.end(), partsums_weights_.begin());
    }


    //! Generate random number following categorical distribution
    /*!
      \param g an uniform random bit generator object
      \return next random category from given distribution 
    */
    template <class G>
    result_type operator()(G& g) const {
        // uniform random number in (0,1)
        const double unif = (g() - g.min())/double(g.max() - g.min());
         
        const double sum_weights = partsums_weights_.back();
        const double discr =  unif * sum_weights;
        
        double part_sum = 0.;
        for(int i=0; i<weights_.size(); ++i) {
            if (discr <= partsums_weights_[i])
               return i;
        }
    }


    //! Get vector of probability weights
    /*!
      \return vector of probability weights 
    */
    std::vector<double> weights() const { return weights_; }


    //! Get minimum potentially generated value
    /*!
      \return minimum potentially generated value 
    */
    result_type min() const { return 0; }
    
    //! Get maximum potentially generated value
    /*!
      \return maximum potentially generated value 
    */
    result_type max() const { return weights_.size(); }

private:
    // vector of weights: no assumption that they are probabilities, i.e. no guarantee sum_weights_ = 1
    std::vector<double> weights_;

    // vector of partial sums of the vector of weights
    std::vector<double> partsums_weights_;

};

} // namespace pserng

#endif//PSERNG_CATEGORICAL_DISTRIBUTION_HPP_
