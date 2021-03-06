// $Id: fe_interface.h 2501 2007-11-20 02:33:29Z benkirk $

// The libMesh Finite Element Library.
// Copyright (C) 2002-2007  Benjamin S. Kirk, John W. Peterson

// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA



#ifndef __fe_interface_h__
#define __fe_interface_h__

// C++ includes
#include <vector>
#include <map>

// Local includes
#include "genius_common.h"
#include "enum_elem_type.h"


// forward declarations
class BoundaryInfo;
//class DofConstraints;
//class DofMap;
class Elem;
class FEType;
class FEComputeData;
//class PeriodicBoundaries;
class Point;
class MeshBase;

/**
 * This class provides an encapsulated access to all @e static
 * public member functions of finite element classes.
 * Using this class, one need not worry about the correct
 * finite element class.
 *
 * @author Daniel Dreyer, 2002-2007
 */

// ------------------------------------------------------------
// FEInterface class definition

class FEInterface
{
private:

  /**
   * Empty constructor. Do not create an object of this type.
   */
  FEInterface();

public:

  /**
   * Destructor.
   */
  virtual ~FEInterface() {return;}

  /**
   * @returns the number of shape functions associated with this
   * finite element of type \p fe_t.
   * Automatically decides which finite element class to use.
   *
   * On a p-refined element, \p fe_t.order should be the total order of the element.
   */
  static unsigned int n_shape_functions(const unsigned int dim,
					const FEType& fe_t,
					const ElemType t);

  /**
   * @returns the number of shape functions associated with this
   * finite element.
   * Automatically decides which finite element class to use.
   *
   * On a p-refined element, \p fe_t.order should be the total order of the element.
   */
  static unsigned int n_dofs(const unsigned int dim,
			     const FEType& fe_t,
			     const ElemType t);

  /**
   * @returns the number of dofs at node n for a finite element
   * of type \p fe_t.
   * Automatically decides which finite element class to use.
   *
   * On a p-refined element, \p fe_t.order should be the total order of the element.
   */
  static unsigned int n_dofs_at_node(const unsigned int dim,
				     const FEType& fe_t,
				     const ElemType t,
				     const unsigned int n);

  /**
   * @returns the number of dofs interior to the element,
   * not associated with any interior nodes.
   * Automatically decides which finite element class to use.
   *
   * On a p-refined element, \p fe_t.order should be the total order of the element.
   */
  static unsigned int n_dofs_per_elem(const unsigned int dim,
				      const FEType& fe_t,
				      const ElemType t);

  /**
   * Fills the vector di with the local degree of freedom indices
   * associated with side \p s of element \p elem
   * Automatically decides which finite element class to use.
   *
   * On a p-refined element, \p fe_t.order should be the base order of the element.
   */
  static void dofs_on_side(const Elem* const elem,
                           const unsigned int dim,
                           const FEType& fe_t,
                           unsigned int s,
                           std::vector<unsigned int>& di);

  /**
   * Fills the vector di with the local degree of freedom indices
   * associated with edge \p e of element \p elem
   * Automatically decides which finite element class to use.
   *
   * On a p-refined element, \p fe_t.order should be the base order of the element.
   */
  static void dofs_on_edge(const Elem* const elem,
                           const unsigned int dim,
                           const FEType& fe_t,
                           unsigned int e,
                           std::vector<unsigned int>& di);

  /**
   * Build the nodal soln from the element soln.
   * This is the solution that will be plotted.
   * Automatically passes the request to the appropriate
   * finite element class member.  To indicate that
   * results from this specific implementation of
   * \p nodal_soln should not be used, the vector
   * \p nodal_soln is returned empty.
   *
   * On a p-refined element, \p fe_t.order should be the base order of the element.
   */
  static void nodal_soln(const unsigned int dim,
			 const FEType& fe_t,
			 const Elem* elem,
			 const std::vector<Number>& elem_soln,
			 std::vector<Number>& nodal_soln);

  /**
   * @returns the location (on the reference element) of the
   * point \p p located in physical space.  This function requires
   * inverting the (probably nonlinear) transformation map, so
   * it is not trivial. The optional parameter \p tolerance defines
   * how close is "good enough."  The map inversion iteration
   * computes the sequence \f$ \{ p_n \} \f$, and the iteration is
   * terminated when \f$ \|p - p_n\| < \mbox{\texttt{tolerance}} \f$
   */
  static Point inverse_map (const unsigned int dim,
			    const FEType& fe_t,
			    const Elem* elem,
			    const Point& p,
			    const Real tolerance = TOLERANCE,
			    const bool secure = true);

  /**
   * @returns the location (on the reference element) of the points \p
   * physical_points located in physical space.  This function
   * requires inverting the (probably nonlinear) transformation map,
   * so it is not trivial. The location of each point on the reference
   * element is returned in the vector \p reference_points. The
   * optional parameter \p tolerance defines how close is "good
   * enough."  The map inversion iteration computes the sequence \f$
   * \{ p_n \} \f$, and the iteration is terminated when \f$ \|p -
   * p_n\| < \mbox{\texttt{tolerance}} \f$
   */
  static void  inverse_map (const unsigned int dim,
			    const FEType& fe_t,
			    const Elem* elem,
			    const std::vector<Point>& physical_points,
			    std::vector<Point>&       reference_points,
			    const Real tolerance = TOLERANCE,
			    const bool secure = true);

  /**
   * @returns true if the point p is located on the reference element
   * for element type t, false otherwise.  Since we are doing floating
   * point comparisons here the parameter \p eps can be specified to
   * indicate a tolerance.  For example, \f$ x \le 1 \f$  becomes
   * \f$ x \le 1 + \epsilon \f$.
   */
  static bool on_reference_element(const Point& p,
				   const ElemType t,
				   const Real eps=1.e-6);
  /**
   * @returns the value of the \f$ i^{th} \f$ shape function at
   * point \p p. This method allows you to specify the dimension,
   * element type, and order directly. Automatically passes the
   * request to the appropriate finite element class member.
   *
   * On a p-refined element, \p fe_t.order should be the total order of the element.
   */
  static Real shape(const unsigned int dim,
		    const FEType& fe_t,
		    const ElemType t,
		    const unsigned int i,
		    const Point& p);

  /**
   * @returns the value of the \f$ i^{th} \f$ shape function at
   * point \p p. This method allows you to specify the dimension,
   * element type, and order directly. Automatically passes the
   * request to the appropriate finite element class member.
   *
   * On a p-refined element, \p fe_t.order should be the base order of the element.
   */
  static Real shape(const unsigned int dim,
		    const FEType& fe_t,
		    const Elem* elem,
		    const unsigned int i,
		    const Point& p);

  /**
   * Lets the appropriate child of \p FEBase compute the requested
   * data for the input specified in \p data, and returns the values
   * also through \p data.  See this as a generalization of \p shape().
   * Currently, with disabled infinite elements, returns a vector of
   * all shape functions of \p elem evaluated ap \p p.
   *
   * On a p-refined element, \p fe_t.order should be the base order of the element.
   */
  static void compute_data(const unsigned int dim,
			   const FEType& fe_t,
			   const Elem* elem,
			   FEComputeData& data);

#ifdef ENABLE_AMR
  /**
   * Computes the constraint matrix contributions (for
   * non-conforming adapted meshes) corresponding to
   * variable number \p var_number.
   */
  //static void compute_constraints (DofConstraints &constraints,
	//			   DofMap &dof_map,
	//			   const unsigned int variable_number,
	//			   const Elem* elem);
#endif // #ifdef ENABLE_AMR



  /**
   * Returns the maximum polynomial degree that the given finite
   * element family can support on the given geometric element.
   */
  static unsigned int max_order (const FEType& fe_t,
			         const ElemType& el_t);

  /**
   * Returns true if separate degrees of freedom must be allocated for
   * vertex DoFs and edge/face DoFs at a hanging node.
   */
  static bool extra_hanging_dofs (const FEType& fe_t);

};





#endif // ifndef __fe_interface_h__
