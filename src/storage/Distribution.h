#ifndef STORM_STORAGE_DISTRIBUTION_H_
#define STORM_STORAGE_DISTRIBUTION_H_

#include <vector>
#include <ostream>
#include <boost/container/flat_map.hpp>

#include "src/storage/sparse/StateType.h"

namespace storm {
    namespace utility {
        template <typename ValueType>
        class ConstantsComparator;
    }
    
    namespace storage {
        
        template<typename ValueType>
        class Distribution {
        public:
            typedef boost::container::flat_map<storm::storage::sparse::state_type, ValueType> container_type;
            typedef typename container_type::iterator iterator;
            typedef typename container_type::const_iterator const_iterator;
            
            /*!
             * Creates an empty distribution.
             */
            Distribution();
            
            /*!
             * Checks whether the two distributions specify the same probabilities to go to the same states.
             *
             * @param other The distribution with which the current distribution is to be compared.
             * @return True iff the two distributions are equal.
             */
            bool equals(Distribution<ValueType> const& other, storm::utility::ConstantsComparator<ValueType> const& comparator = storm::utility::ConstantsComparator<ValueType>()) const;
            
            /*!
             * Assigns the given state the given probability under this distribution.
             *
             * @param state The state to which to assign the probability.
             * @param probability The probability to assign.
             */
            void addProbability(storm::storage::sparse::state_type const& state, ValueType const& probability);
            
            /*!
             * Removes the given probability mass of going to the given state.
             *
             * @param state The state for which to remove the probability.
             * @param probability The probability to remove.
             * @param comparator A comparator that is used to determine if the remaining probability is zero. If so, the
             * entry is removed.
             */
            void removeProbability(storm::storage::sparse::state_type const& state, ValueType const& probability, storm::utility::ConstantsComparator<ValueType> const& comparator = storm::utility::ConstantsComparator<ValueType>());
            
            /*!
             * Removes the probability mass from one state and adds it to another.
             *
             * @param fromState The state from which to take the probability mass.
             * @param toState The state from which to which to add the probability mass.
             * @param probability The probability mass to shift.
             * @param comparator A comparator that is used to determine if the remaining probability is zero. If so, the
             * entry is removed.
             */
            void shiftProbability(storm::storage::sparse::state_type const& fromState, storm::storage::sparse::state_type const& toState, ValueType const& probability, storm::utility::ConstantsComparator<ValueType> const& comparator = storm::utility::ConstantsComparator<ValueType>());
            
            /*!
             * Retrieves an iterator to the elements in this distribution.
             *
             * @return The iterator to the elements in this distribution.
             */
            iterator begin();
            
            /*!
             * Retrieves an iterator to the elements in this distribution.
             *
             * @return The iterator to the elements in this distribution.
             */
            const_iterator begin() const;
            
            /*!
             * Retrieves an iterator past the elements in this distribution.
             *
             * @return The iterator past the elements in this distribution.
             */
            iterator end();
            
            /*!
             * Retrieves an iterator past the elements in this distribution.
             *
             * @return The iterator past the elements in this distribution.
             */
            const_iterator end() const;
            
            /*!
             * Scales the distribution by multiplying all the probabilities with 1/p where p is the probability of moving
             * to the given state and sets the probability of moving to the given state to zero. If the probability is
             * already zero, this operation has no effect.
             *
             * @param state The state whose associated probability is used to scale the distribution.
             */
            void scale(storm::storage::sparse::state_type const& state);
            
            /*!
             * Retrieves the hash value of the distribution.
             *
             * @return The hash value of the distribution.
             */
            std::size_t getHash() const;
            
            /*!
             * Retrieves the size of the distribution, i.e. the size of the support set.
             */
            std::size_t size() const;
            
            bool less(Distribution<ValueType> const& other, storm::utility::ConstantsComparator<ValueType> const& comparator) const;
            
        private:
            // A list of states and the probabilities that are assigned to them.
            container_type distribution;
            
            // A hash value that is maintained to allow for quicker equality comparison between distributions.
            std::size_t hash;
        };
        
        template<typename ValueType>
        std::ostream& operator<<(std::ostream& out, Distribution<ValueType> const& distribution);
    }
}

namespace std {
    
    template <typename ValueType>
    struct hash<storm::storage::Distribution<ValueType>> {
        std::size_t operator()(storm::storage::Distribution<ValueType> const& distribution) const {
            return (distribution.getHash());
        }
    };
    
}


#endif /* STORM_STORAGE_DISTRIBUTION_H_ */