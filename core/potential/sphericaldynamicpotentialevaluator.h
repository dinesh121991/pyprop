#ifndef SPHERICALDYNAMICPOTENTIALEVALUATOR_H
#define SPHERICALDYNAMICPOTENTIALEVALUATOR_H

#include "../common.h"
#include "../wavefunction.h"
#include "../representation/sphericalrepresentation.h"
#include "staticpotential.h"
#include "potentialbase.h"
#include "potentialaction.h"

// The class inherits from DynamicPotentialClass and ActionClass
// DynamicPotentialClass defines the potential
// ActionClass defines the how the potential is to be applied to the data
template<class ActionClass, int Rank>
class SphericalDynamicPotentialEvaluatorBase : public ActionClass
{
public:
	/** Updates propagates the wavefunction a step with this dynamic potential **/
	template<class DynamicPotentialClass>
	void IterateAction(DynamicPotentialClass &potential, const Wavefunction<Rank> &psi, blitz::Array<cplx, Rank> updateData, const cplx &timeStep, const double &curTime)
	{
		typedef SphericalRepresentation<Rank> SphRepr;		

		//Set up PotentialClass
		potential.CurTime = curTime;
		potential.TimeStep = timeStep;

		//Get representations
		SphRepr *repr = static_cast<SphRepr*>(&psi.GetRepresentation());

		blitz::TinyVector< blitz::Array<double, 1>, Rank-1 > grid;
		for (int i=0; i<Rank-1; i++)
		{
			grid(i).reference(repr->GetLocalGrid(i));
		}
		
		blitz::Array<double, 2> omegaGrid;
		omegaGrid.reference(repr->GetLocalAngularGrid());
		
		//postition is size rank+1 since <r,(l,m)> --> <r,l,m> = pos
		blitz::TinyVector<double, Rank+1> pos;
		typename blitz::Array<cplx, Rank>::iterator it = updateData.begin();
		for (int linearCount=0; linearCount<updateData.size(); linearCount++)
		{
			for (int i=0; i<Rank-1; i++)
			{
				pos(i) = grid(i)(it.position()(i));
			}
			
			// second last rank - theta
			int omegaIndex = it.position()(Rank-1);
			pos(Rank-1) = omegaGrid(omegaIndex, 0);
							
			// last rank - phi
			pos(Rank) = omegaGrid(omegaIndex, 1);

			// Uses the function from the inherited classes
			ApplyAction(it, potential.GetPotentialValue(pos), timeStep );
			it++;
		}
	}
};

template <class PotentialClass, int Rank>
class SphericalDynamicPotentialEvaluator
{
public:
	typedef ApplyPotentialClass<Rank> ApplyClass;
	typedef UpdatePotentialClass<Rank> UpdateClass;
	typedef GetPotentialClass<Rank> GetClass;
	
	PotentialClass Potential;
	SphericalDynamicPotentialEvaluatorBase< ApplyClass, Rank> Apply;
	SphericalDynamicPotentialEvaluatorBase< UpdateClass, Rank> Update;
	SphericalDynamicPotentialEvaluatorBase< GetClass, Rank> Get;

	
	void ApplyConfigSection(const ConfigSection &config)
	{
		Potential.ApplyConfigSection(config);
	}

	/** Updates propagates the wavefunction a step with this dynamic potential **/
	void ApplyPotential(Wavefunction<Rank> &psi, const cplx &timeStep, const double &curTime)
	{
		Apply.IterateAction(Potential, psi, psi.GetData(), timeStep, curTime);
	}

	/** Updates a static potential with the expotential of the potential of this dynamic potential. */
	void UpdateStaticPotential(StaticPotential<Rank> &potential, const Wavefunction<Rank> &psi, const cplx &timeStep, const double &curTime)
	{
		blitz::Array<cplx, Rank> potentialData( potential.GetPotentialData() );
		Update.IterateAction(Potential, psi, potentialData, timeStep, curTime);
	}

	/** 
   	  * Returns an array containing this dynamic potential at the given time. 
	  * WARNING: This method allocates and returns a new blitz array in the same size as the wavefunction
	  */
	blitz::Array<cplx, Rank> GetPotential(const Wavefunction<Rank> &psi, const cplx &timeStep, const double &curTime)
	{
		//TODO: Fix for multiproc
		blitz::Array<cplx, Rank> potentialData(psi.Data.shape());
		Get.IterateAction(Potential, psi, potentialData, timeStep, curTime);
		return potentialData;
	}

	double CalculateExpectationValue(const Wavefunction<Rank> &psi, const cplx &timeStep, const double curTime)
	{
		typedef SphericalRepresentation<Rank> SphRepr;		
	
		blitz::Array<cplx, Rank> data(psi.Data);
		
		//Set up PotentialClass
		Potential.CurTime = curTime;
		Potential.TimeStep = timeStep;

		//Get representations
		SphRepr *repr = static_cast<SphRepr*>(&psi.GetRepresentation());

		blitz::TinyVector< blitz::Array<double, 1>, Rank > grid;
		for (int i=0; i<Rank; i++)
		{
			grid(i).reference(repr->GetLocalGrid(i));
		}
		blitz::Array<double, 2> omegaGrid(repr->GetLocalAngularGrid());

		blitz::TinyVector< blitz::Array<double, 1>, Rank > weights;
		for (int i=0; i<Rank; i++)
		{
			weights(i).reference(repr->GetLocalWeights(i));
		}

		blitz::TinyVector<double, Rank+1> pos;
		typename blitz::Array<cplx, Rank>::iterator it = data.begin();
		double weight = 1;
		double expValue = 0;
		for (int linearCount=0; linearCount<data.size(); linearCount++)
		{
			weight = 1;
			for (int i=0; i<Rank-1; i++)
			{
				pos(i) = grid(i)(it.position()(i));
				weight *= weights(i)(it.position()(i));
			}
			weight *= weights(Rank-1)(it.position()(Rank-1));

			// second last rank - theta
			int omegaIndex = it.position()(Rank-1);
			pos(Rank-1) = omegaGrid(omegaIndex, 0);
							
			// last rank - phi
			pos(Rank) = omegaGrid(omegaIndex, 1);		
			
			expValue += weight * real((*it) * conj(*it) * Potential.GetPotentialValue(pos));
			it++;
		}
		return expValue;
	}

};

#endif
