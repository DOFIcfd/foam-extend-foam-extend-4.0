/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | foam-extend: Open Source CFD
   \\    /   O peration     | Version:     4.0
    \\  /    A nd           | Web:         http://www.foam-extend.org
     \\/     M anipulation  | For copyright notice see file Copyright
-------------------------------------------------------------------------------
License
    This file is part of foam-extend.

    foam-extend is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    foam-extend is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with foam-extend.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "outputFilterOutputControl.H"

// * * * * * * * * * * * * * Static Member Data  * * * * * * * * * * * * * * //

template<>
const char* Foam::NamedEnum
<
    Foam::outputFilterOutputControl::outputControls,
    2
>::names[] =
{
    "timeStep",
    "outputTime"
};

const Foam::NamedEnum<Foam::outputFilterOutputControl::outputControls, 2>
    Foam::outputFilterOutputControl::outputControlNames_;


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::outputFilterOutputControl::outputFilterOutputControl
(
    const Time& t,
    const dictionary& dict
)
:
    time_(t),
    outputControl_(ocTimeStep),
    outputInterval_(0)
{
    read(dict);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::outputFilterOutputControl::~outputFilterOutputControl()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::outputFilterOutputControl::read(const dictionary& dict)
{
    outputControl_ = outputControlNames_.read(dict.lookup("outputControl"));

    switch (outputControl_)
    {
        case ocTimeStep:
        {
            dict.lookup("outputInterval") >> outputInterval_;
        }
        default:
        {
            // do nothing
        }
    }
}


bool Foam::outputFilterOutputControl::output() const
{
    switch (outputControl_)
    {
        case ocTimeStep:
        {
            return
            (
                (outputInterval_ <= 1)
             || !(time_.timeIndex() % outputInterval_)
            );
            break;
        }
        case ocOutputTime:
        {
            return time_.outputTime();
            break;
        }
        default:
        {
            FatalErrorIn("bool Foam::outputFilterOutputControl::output()")
                << "Unknown output control: "
                << outputControlNames_[outputControl_] << nl
                << abort(FatalError);
        }
    }

    return false;
}


// ************************************************************************* //
