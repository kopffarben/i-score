#include "FullViewConstraintViewModel.hpp"

#include "Document/Constraint/ViewModels/AbstractConstraintViewModel.hpp"
#include "Document/Constraint/ViewModels/AbstractConstraintViewModelSerialization.hpp"

template<>
void Visitor<Reader<DataStream>>::readFrom (const FullViewConstraintViewModel& constraint)
{
    readFrom (static_cast<const AbstractConstraintViewModel&> (constraint) );
}

template<>
void Visitor<Reader<JSON>>::readFrom (const FullViewConstraintViewModel& constraint)
{
    readFrom (static_cast<const AbstractConstraintViewModel&> (constraint) );
}
