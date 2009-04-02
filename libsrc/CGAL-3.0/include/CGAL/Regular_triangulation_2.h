// Copyright (c) 1997  INRIA Sophia-Antipolis (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you may redistribute it under
// the terms of the Q Public License version 1.0.
// See the file LICENSE.QPL distributed with CGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $Source: /CVSROOT/CGAL/Packages/Triangulation_2/include/CGAL/Regular_triangulation_2.h,v $
// $Revision: 1.69 $ $Date: 2003/09/18 10:26:06 $
// $Name: current_submission $
//
// Author(s)     : Frederic Fichel, Mariette Yvinec, Julia Floetotto

#ifndef CGAL_REGULAR_TRIANGULATION_2_H
#define CGAL_REGULAR_TRIANGULATION_2_H

#include <CGAL/Triangulation_2.h>
#include <CGAL/Regular_triangulation_face_base_2.h>
#include <CGAL/Regular_triangulation_vertex_base_2.h>
//#include <CGAL/Triangulation_iterator_adaptator.h>

CGAL_BEGIN_NAMESPACE

template < class Gt,
           class Tds  = Triangulation_data_structure_2 <
                        Regular_triangulation_vertex_base_2<Gt>,
		        Regular_triangulation_face_base_2<Gt> > >
class Regular_triangulation_2 : public Triangulation_2<Gt,Tds>
{
  typedef Regular_triangulation_2<Gt, Tds>     Self;
  typedef Triangulation_2<Gt,Tds>              Base;
public:
  typedef Tds                                  Triangulation_data_structure;
  typedef Gt                                   Geom_traits;
  typedef typename Gt::Bare_point              Bare_point;
  typedef typename Gt::Weighted_point          Weighted_point;
  typedef typename Gt::Weighted_point          Point;
  typedef typename Gt::Weight                  Weight;

  typedef typename Base::Face_handle           Face_handle;
  typedef typename Base::Vertex_handle         Vertex_handle;
  typedef typename Base::Vertex                Vertex;
  typedef typename Base::Edge                  Edge;
  typedef typename Base::Locate_type           Locate_type;
  typedef typename Base::Face_circulator       Face_circulator;
  typedef typename Base::Edge_circulator       Edge_circulator;
  typedef typename Base::Vertex_circulator     Vertex_circulator;
  typedef typename Base::Finite_edges_iterator Finite_edges_iterator;
  typedef typename Base::All_edges_iterator    All_edges_iterator;
  typedef typename Base::Finite_faces_iterator Finite_faces_iterator;
  typedef typename Base::All_faces_iterator    All_faces_iterator;
  typedef typename Base::Face::Vertex_list     Vertex_list;
  typedef typename Vertex_list::iterator       Vertex_list_iterator;

private:
  typedef std::list<Face_handle>      Faces_around_stack;

  class Hidden_tester {
  public:
    bool operator()(const typename Base::All_vertices_iterator&  it){
      return it->is_hidden();
     }
    bool operator()(const typename Base::Finite_vertices_iterator&  it){
      return it->is_hidden();
    }
  };

  class Unhidden_tester {
  public:
    bool operator()(const typename Base::Finite_vertices_iterator&  it){
      return ! it->is_hidden();
    }
  };

  typedef typename Base::All_vertices_iterator     All_vib;
  typedef typename Base::Finite_vertices_iterator  Finite_vib;

public:
  // We derive in order to add a conversion to handle.
  class All_vertices_iterator :
    public Filter_iterator<All_vib, Hidden_tester> {
    typedef Filter_iterator<All_vib, Hidden_tester> Base;
    typedef All_vertices_iterator                     Self;
     public:
    All_vertices_iterator() : Base() {}
    All_vertices_iterator(const Base &b) : Base(b) {}
    Self & operator++() { Base::operator++(); return *this; }
    Self & operator--() { Base::operator--(); return *this; }
    Self operator++(int) { Self tmp(*this); ++(*this); return tmp; }
    Self operator--(int) { Self tmp(*this); --(*this); return tmp; }
    operator Vertex_handle() const { return Base::base(); }
  };

  class Finite_vertices_iterator :
    public Filter_iterator<Finite_vib, Hidden_tester> {
    typedef Filter_iterator<Finite_vib, Hidden_tester> Base;
    typedef Finite_vertices_iterator                          Self;
  public:
    Finite_vertices_iterator() : Base() {}
    Finite_vertices_iterator(const Base &b) : Base(b) {}
    Self & operator++() { Base::operator++(); return *this; }
    Self & operator--() { Base::operator--(); return *this; }
    Self operator++(int) { Self tmp(*this); ++(*this); return tmp; }
    Self operator--(int) { Self tmp(*this); --(*this); return tmp; }
    operator Vertex_handle() const { return Base::base(); }
 };

  class Hidden_vertices_iterator :
    public Filter_iterator<Finite_vib, Unhidden_tester> {
    typedef Filter_iterator<Finite_vib, Unhidden_tester> Base;
    typedef Hidden_vertices_iterator                     Self;
  public:
    Hidden_vertices_iterator() : Base() {}
    Hidden_vertices_iterator(const Base &b) : Base(b) {}
    Self & operator++() { Base::operator++(); return *this; }
    Self & operator--() { Base::operator--(); return *this; }
    Self operator++(int) { Self tmp(*this); ++(*this); return tmp; }
    Self operator--(int) { Self tmp(*this); --(*this); return tmp; }
    operator Vertex_handle() const { return Base::base(); }
 };

//  typedef Filter_iterator<All_vib,Hidden_tester>      All_filtered;
//   typedef Filter_iterator<Finite_vib,Hidden_tester>   Finite_filtered;
//   typedef Filter_iterator<Finite_vib,Unhidden_tester> Hidden_filtered;

// public:
//   typedef Triangulation_iterator_handle_adaptor
//                   <All_filtered, Vertex_handle>    All_vertices_iterator;
//   typedef Triangulation_iterator_handle_adaptor
//                <Finite_filtered, Vertex_handle>    Finite_vertices_iterator;
//   typedef Triangulation_iterator_handle_adaptor
//                 <Hidden_filtered,Vertex_handle>    Hidden_vertices_iterator;

private:
  int _hidden_vertices;

public:
  Regular_triangulation_2(const Gt& gt=Gt()) : Base(gt), _hidden_vertices(0) {}
  Regular_triangulation_2(const Regular_triangulation_2 &rt);

  Regular_triangulation_2 & operator=(const Regular_triangulation_2 &tr);

  int number_of_vertices() const {
    return Base::number_of_vertices() - _hidden_vertices;
  }

  int number_of_hidden_vertices() const {
    return _hidden_vertices;
  }

  // CHECK - QUERY
  Oriented_side power_test(const Weighted_point &p,
			   const Weighted_point &q,
			   const Weighted_point &r,
			   const Weighted_point &s) const;
  Oriented_side power_test(const Weighted_point &p,
			   const Weighted_point &q,
			   const Weighted_point &r) const;
  Oriented_side power_test(const Weighted_point &p,
			   const Weighted_point &r) const;
  Oriented_side power_test(const Face_handle &f,
			   const Weighted_point &p) const;
  Oriented_side power_test(const Face_handle& f, int i,
			   const Weighted_point &p) const;


  bool is_valid(bool verbose = false, int level = 0) const;
  void show_face(Face_handle fh) const;
  void show_all() const;

  // DUAL
  Bare_point dual (Face_handle f) const;
  Object dual(const Edge &e) const ;
  Object dual(const Edge_circulator& ec) const;
  Object dual(const Finite_edges_iterator& ei) const;
  Bare_point weighted_circumcenter(Face_handle f) const;
  Bare_point weighted_circumcenter(const Weighted_point& p0,
			      const Weighted_point& p1,
			      const Weighted_point& p2) const;

  // Insertion, Deletion and Flip
  Vertex_handle push_back(const Weighted_point &p);
  Vertex_handle insert(const Weighted_point &p,
		       Face_handle f = Face_handle(NULL) );
  Vertex_handle insert(const Weighted_point &p,
	 	       Locate_type  lt,
		       Face_handle loc, int li );
  Vertex_handle insert_in_face(const Weighted_point &p, Face_handle f);
  Vertex_handle insert_in_edge(const Weighted_point &p, Face_handle f, int i);
  void flip(Face_handle f, int i);
  void remove_degree_3(Vertex_handle v,
		       Face_handle f = Face_handle(NULL));
  void remove(Vertex_handle v);

  All_vertices_iterator all_vertices_begin () const;
  All_vertices_iterator all_vertices_end () const;

  Finite_vertices_iterator finite_vertices_begin () const;
  Finite_vertices_iterator finite_vertices_end () const;

  Hidden_vertices_iterator hidden_vertices_begin () const;
  Hidden_vertices_iterator hidden_vertices_end () const;

  //  Vertex_handle file_input(std::istream& is);
  // void file_output(std::ostream& os) const;

public:
  void clear();
  void copy_triangulation(const Self& tr);
private:
  Vertex_handle reinsert(Vertex_handle v, Face_handle start);
  void regularize(Vertex_handle v);
  void remove_hidden(Vertex_handle v);
  void remove_2D(Vertex_handle v);
  void fill_hole_regular(std::list<Edge> & hole);
  void set_face(Vertex_list& vl, const Face_handle& fh);
  void update_hidden_points_3_1(const Face_handle& f1, const Face_handle& f2,
				const Face_handle& f3);
  void update_hidden_points_2_2(const Face_handle& f1, const Face_handle& f2);
  void update_hidden_points_1_3(const Face_handle& f1, const Face_handle& f2,
				const Face_handle& f3);

  Vertex_handle hide_new_vertex(Face_handle f, const Weighted_point& p);
  void hide_remove_degree_3(Face_handle fh, Vertex_handle vh);
  void hide_vertex(Face_handle f, Vertex_handle v);
   void exchange_incidences(Vertex_handle va, Vertex_handle vb);
  void exchange_hidden(Vertex_handle va, Vertex_handle vb);

  void stack_flip(Vertex_handle v, Faces_around_stack &faces_around);
  void stack_flip_4_2(Face_handle f, int i, int j,
		      Faces_around_stack &faces_around);
  void stack_flip_3_1(Face_handle f, int i, int j,
		      Faces_around_stack &faces_around);
  void stack_flip_2_2(Face_handle f, int i,
		      Faces_around_stack &faces_around);
  void stack_flip_dim1(Face_handle f, int i,
		       Faces_around_stack &faces_around);
  bool is_valid_face(Face_handle fh) const;
  bool is_valid_vertex(Vertex_handle fh) const;



public:
  template < class InputIterator >
  int
  insert(InputIterator first, InputIterator last)
  {
      int n = number_of_vertices();
      while(first != last)
      {
          insert(*first);
  	  ++first;
      }
      return number_of_vertices() - n;
  }

  template < class Stream>
  Stream& draw_dual(Stream & ps) const
    {
      Finite_edges_iterator eit= finite_edges_begin();
      for (; eit != finite_edges_end(); ++eit) {
	Object o = dual(eit);
	typename Geom_traits::Line_2  l;
	typename Geom_traits::Ray_2   r;
	typename Geom_traits::Segment_2 s;
	if (CGAL::assign(s,o)) ps << s;
	if (CGAL::assign(r,o)) ps << r;
	if (CGAL::assign(l,o)) ps << l;
      }
      return ps;
    }

};

template < class Gt, class Tds >
void
Regular_triangulation_2<Gt,Tds>::
clear()
{
  Base::clear();
  _hidden_vertices = 0;
}

template < class Gt, class Tds >
void
Regular_triangulation_2<Gt,Tds>::
copy_triangulation(const Self &tr )
{
  Base::copy_triangulation(tr);
  _hidden_vertices = tr._hidden_vertices;
  // the list of vertices have been copied member for member and are
  // not good
  // clear them and next
  // scan the hidden vertices to retablish the list in faces
  typename Tds::Face_iterator
                       baseit= this->_tds.face_iterator_base_begin();
  for( ; baseit !=  this->_tds.face_iterator_base_end(); baseit++){
    baseit->vertex_list().clear();
  }
  Hidden_vertices_iterator hvit = hidden_vertices_begin();
  for( ; hvit !=  hidden_vertices_end() ; ++hvit){
    hvit->face()->vertex_list().push_back(hvit);
  }
  CGAL_triangulation_postcondition(is_valid());
  return;
}

template < class Gt, class Tds >
Regular_triangulation_2<Gt,Tds>::
Regular_triangulation_2(const Self &tr)
{
  copy_triangulation(tr);
}

template <class Gt, class Tds >
Regular_triangulation_2<Gt,Tds> &
Regular_triangulation_2<Gt, Tds>::
operator=(const Self &tr)
{
  copy_triangulation(tr);
  return *this;
}

template < class Gt, class Tds >
Oriented_side
Regular_triangulation_2<Gt,Tds>::
power_test(const Face_handle &f, const Weighted_point &p) const
{
  // p is supposed to be a finite point
  // if f is a finite face,
  // return  ON_NEGATIVE_SIDE if p is above f
  // (p has to be hidden)
  if (dimension() == 1) return power_test(f->vertex(0)->point(),
					  f->vertex(1)->point(),p);
  int i;
  if ( ! f->has_vertex(infinite_vertex(), i) )
    return power_test(f->vertex(0)->point(),
		      f->vertex(1)->point(),
		      f->vertex(2)->point(),p);

  Orientation o = orientation(f->vertex(ccw(i))->point(),
			      f->vertex( cw(i))->point(),
			      p);
  if (o==COLLINEAR)
    return power_test(f->vertex(ccw(i))->point(),
		      f->vertex( cw(i))->point(),p);

  return Oriented_side(o);
}

template < class Gt, class Tds >
Oriented_side
Regular_triangulation_2<Gt,Tds>::
power_test(const Face_handle& f, int i,
	   const Weighted_point &p) const
{
  // f,i is supposed to be a finite edge
  // p is supposed to be on  edge (f,i)
  // return ON_NEGATIVE_SIDE if p is above (f,i)
  // (p has to be hidden)
  CGAL_triangulation_precondition (!is_infinite(f,i) &&
	     orientation(f->vertex(ccw(i))->point(),
			 f->vertex( cw(i))->point(),
			 p) == COLLINEAR);
  return  power_test(f->vertex(ccw(i))->point(),
		     f->vertex( cw(i))->point(),
		     p);
}

template < class Gt, class Tds >
inline
Oriented_side
Regular_triangulation_2<Gt,Tds>::
power_test(const Weighted_point &p,
	   const Weighted_point &q,
	   const Weighted_point &r,
	   const Weighted_point &s) const
{
  return geom_traits().power_test_2_object()(p,q,r,s);
}

template < class Gt, class Tds >
inline
Oriented_side
Regular_triangulation_2<Gt,Tds>::
power_test(const Weighted_point &p,
	   const Weighted_point &q,
	   const Weighted_point &r) const
{
  return geom_traits().power_test_degenerated_2_object()(p,q,r);
}

template < class Gt, class Tds >
inline
Oriented_side
Regular_triangulation_2<Gt,Tds>::
power_test(const Weighted_point &p,
	   const Weighted_point &r) const
{
  return geom_traits().power_test_degenerated_2_object()(p,r);
}

template < class Gt, class Tds >
bool
Regular_triangulation_2<Gt,Tds>::
is_valid_face(Face_handle fh) const
{
  bool result = true;
  if(is_infinite(fh)) result = result && fh->vertex_list().empty();
  if (!result) { show_face(fh);}
  CGAL_triangulation_assertion(result);

  typename Vertex_list::iterator vlit = fh->vertex_list().begin(),
	                       vldone = fh->vertex_list().end();
  for (; vlit != vldone; vlit++)    {
    result = result && power_test(fh, (*vlit)->point()) == ON_NEGATIVE_SIDE;
    result = result && ((*vlit)->face() == fh);
    if (!result)     show_face(fh);
    CGAL_triangulation_assertion(result);
  }
  return result;
}

template < class Gt, class Tds >
bool
Regular_triangulation_2<Gt,Tds>::
is_valid_vertex(Vertex_handle vh) const
{
  bool result = true;
  if (vh->is_hidden()) {
    Locate_type lt;
    int li;
    result = result && (!is_infinite(vh->face()));
    Face_handle loc  = locate(vh->point(), lt, li, vh->face());
    result = result && (loc == vh->face() ||
		        (lt == Base::VERTEX &&
			     vh->face()->has_vertex(loc->vertex(li))) ||
			(lt == Base::EDGE && vh->face() ==
			 loc->neighbor(li)) );
        if ( !result) {
      std::cerr << vh->point() << " " << std::endl;
      std::cerr << "vh_>face " << &*(vh->face())  << " " << std::endl;
      std::cerr <<  "loc      " <<  &*(loc )
	        << " lt " << lt  << " li " << li << std::endl;
      show_face(vh->face());
      show_face(loc);
    }
    CGAL_triangulation_assertion(result);
    result = result &&
             power_test(vh->face(),vh->point()) == ON_NEGATIVE_SIDE;
    CGAL_triangulation_assertion(result);
  }
  else { // normal vertex
    result = result && vh->face()->has_vertex(vh);
    CGAL_triangulation_assertion(result);
  }
  return result;
}

template < class Gt, class Tds >
bool
Regular_triangulation_2<Gt,Tds>::
is_valid(bool verbose, int level) const
{
  // cannot call for is_valid() of Base Triangulation class
  // because 1) number of vertices of base class does not match
  // tds.is_valid calls is_valid for each vertex
  // and the test is not fullfilled by  hidden vertices ...
  // result = result && Triangulation_2<Gt,Tds>::is_valid(verbose, level);
  bool result = true;
  for(All_faces_iterator fit = all_faces_begin();
      fit != all_faces_end(); ++fit) {
    result = result && is_valid_face(fit);
  }

  for(All_vertices_iterator vit = all_vertices_begin();
                            vit != all_vertices_end(); ++vit) {
    result = result && is_valid_vertex(vit);
  }

   for(Hidden_vertices_iterator hvit = hidden_vertices_begin();
                                hvit != hidden_vertices_end(); ++hvit) {
    result = result && is_valid_vertex(hvit);
  }

   switch(dimension()) {
   case 0 :
     break;
   case 1:
     if (number_of_vertices() > 2 ) {
       Finite_vertices_iterator it1 = finite_vertices_begin(),
	 it2(it1), it3(it1);
       ++it2;
       ++it3; ++it3;
       while( it3 != finite_vertices_end()) {
	 Orientation s = orientation(it1->point(),
				    it2->point(),
				    it3->point());
	 result = result && s == COLLINEAR ;
	 CGAL_triangulation_assertion(result);
	 ++it1 ; ++it2; ++it3;
       }
     }
     break;
   case 2 :
    for(Finite_faces_iterator it=finite_faces_begin();
	 it!=finite_faces_end(); it++) {
      CGAL_triangulation_assertion( ! is_infinite(it));
      Orientation s = orientation(it->vertex(0)->point(),
				  it->vertex(1)->point(),
				  it->vertex(2)->point());
      CGAL_triangulation_assertion( s == LEFT_TURN );
      result = result && ( s == LEFT_TURN );

      for (int i = 0 ; i < 3 ; i++) {
	if (!is_infinite(it->vertex(i)))
	  result = result && ON_POSITIVE_SIDE !=
	    power_test(it->neighbor(i), it->vertex(i)->point());
	CGAL_triangulation_assertion(result);
      }
    }

     Vertex_circulator start = infinite_vertex()->incident_vertices();
     Vertex_circulator pc(start);
     Vertex_circulator qc(start); ++qc;
     Vertex_circulator rc(start); ++rc; ++rc;
     do{
       Orientation s = orientation(pc->point(),
				   qc->point(),
				   rc->point());
       CGAL_triangulation_assertion( s != LEFT_TURN );
       result = result && ( s != LEFT_TURN );
       ++pc ; ++qc ; ++rc;
     } while(pc != start);

     // check number of faces. This cannot be done by the Tds
     // which does not know the number of components nor the genus
     result = result && (number_of_faces() == 2*(number_of_vertices()+1)
		                            - 4
                                           - infinite_vertex()->degree());
     CGAL_triangulation_assertion( result);
     break;
   }

   // in any dimension
   if(verbose) {
     std::cerr << " nombres de sommets " << number_of_vertices() << "\t"
	       << "nombres de sommets  caches " << number_of_hidden_vertices()
	       << std::endl;
   }
   result = result && ( Base::number_of_vertices() ==
			number_of_vertices() + number_of_hidden_vertices());
   CGAL_triangulation_assertion( result);
   return result;
}


template <class Gt, class Tds >
void
 Regular_triangulation_2<Gt, Tds>::
show_face(Face_handle fh) const
{
  Base::show_face(fh);

  typename Vertex_list::iterator current;
  std::cerr << "  +++++>>>    ";
  for (current= fh->vertex_list().begin();
       current!= fh->vertex_list().end() ; current++ ) {
        std::cerr <<"[ "<< ((*current)->point()) << " ] ,  ";
  }
  std::cerr <<std::endl;
}


template < class Gt, class Tds >
void
Regular_triangulation_2<Gt,Tds>::
show_all() const
{
  std::cerr<< "AFFICHE TOUTE LA TRIANGULATION :" << std::endl;
  std::cerr << std::endl<<"====> "<< this ;
  std::cerr <<  " dimension " <<  dimension() << std::endl;
  std::cerr << "nb of vertices " << number_of_vertices()
	    << " nb of hidden vertices " << number_of_hidden_vertices()
	    <<   std::endl;

  if (dimension() < 1) return;
  if(dimension() == 1) {
    std::cerr<<" all edges "<<std::endl;
    All_edges_iterator aeit;
    for(aeit = all_edges_begin(); aeit != all_edges_end(); aeit++){
      show_face(aeit->first);
    }
   }

  else{ //dimension ==2
    std::cerr<<" faces finies "<<std::endl;
    Finite_faces_iterator fi;
    for(fi = finite_faces_begin(); fi != finite_faces_end(); fi++) {
      show_face(fi);
    }

    std::cerr <<" faces infinies "<<std::endl;
    All_faces_iterator afi;
    for(afi = all_faces_begin(); afi != all_faces_end(); afi++) {
      if(is_infinite(afi)) show_face(afi);
    }
  }

  if (number_of_vertices()>1) {
    std::cerr << "affichage des sommets de la triangulation reguliere"
	      <<std::endl;
    All_vertices_iterator vi;
    for( vi = all_vertices_begin(); vi != all_vertices_end(); vi++){
      show_vertex(vi);
      std::cerr << "  / face associee : "
	     <<  &*(vi->face()) << std::endl;
      }
      std::cerr<<std::endl;
  }

   std::cerr << "sommets caches "  << std::endl;
   Hidden_vertices_iterator hvi = hidden_vertices_begin();
   for( ; hvi != hidden_vertices_end(); hvi++) {
     show_vertex(hvi);
      std::cerr << "  / face associee : "
	     << &*(hvi->face()) << std::endl;
   }
  return;
}



//DUALITY
template < class Gt, class Tds >
inline
typename Regular_triangulation_2<Gt,Tds>::Bare_point
Regular_triangulation_2<Gt,Tds>::
dual (Face_handle f) const
{
  return weighted_circumcenter(f);
}

template < class Gt, class Tds >
inline
typename Regular_triangulation_2<Gt,Tds>::Bare_point
Regular_triangulation_2<Gt,Tds>::
weighted_circumcenter(Face_handle f) const
{
  CGAL_triangulation_precondition (dimension()==2 || !is_infinite(f));
  return weighted_circumcenter(f->vertex(0)->point(),
			       f->vertex(1)->point(),
			       f->vertex(2)->point());
}

template<class Gt, class Tds>
inline
typename Regular_triangulation_2<Gt,Tds>::Bare_point
Regular_triangulation_2<Gt,Tds>::
weighted_circumcenter(const Weighted_point& p0,
		      const Weighted_point& p1,
		      const Weighted_point& p2) const
{
  return
    geom_traits().construct_weighted_circumcenter_2_object()(p0,p1,p2);
}

template < class Gt, class Tds >
inline
Object
Regular_triangulation_2<Gt,Tds>::
dual(const Edge &e) const
{
  typedef typename Geom_traits::Line_2        Line;
  typedef typename Geom_traits::Ray_2         Ray;
  typedef typename Geom_traits::Segment       Segment;

  CGAL_triangulation_precondition (! is_infinite(e));
  if( dimension()== 1 ){
    const Weighted_point& p = (e.first)->vertex(cw(e.second))->point();
    const Weighted_point& q = (e.first)->vertex(ccw(e.second))->point();
    Line l  = geom_traits().construct_radical_axis_2_object()(p,q);
    return make_object(l);
  }

  // dimension==2
  if( (! is_infinite(e.first)) &&
      (! is_infinite(e.first->neighbor(e.second))) ) {
    Segment s = geom_traits().construct_segment_2_object()
      (dual(e.first),dual(e.first->neighbor(e.second)));
    return make_object(s);
  }

  // one of the adjacent face is infinite
  Face_handle f; int i;
  if ( is_infinite(e.first)) {
    f=e.first->neighbor(e.second); f->has_neighbor(e.first,i);
  }
  else {
    f=e.first; i=e.second;
  }
  const Weighted_point& p = f->vertex( cw(i))->point();
  const Weighted_point& q = f->vertex( ccw(i))->point();
  Line l  = geom_traits().construct_radical_axis_2_object()(p,q);
  Ray r = geom_traits().construct_ray_2_object()(dual(f), l);
  return make_object(r);
}


template < class Gt, class Tds >
inline
Object
Regular_triangulation_2<Gt,Tds>::
dual(const Edge_circulator& ec) const
{
  return dual(*ec);
}

template < class Gt, class Tds >
inline
Object
Regular_triangulation_2<Gt,Tds>::
dual(const Finite_edges_iterator& ei) const
{
  return dual(*ei);
}

//INSERTION-REMOVAL
template < class Gt, class Tds >
typename Regular_triangulation_2<Gt,Tds>::Vertex_handle
Regular_triangulation_2<Gt,Tds>::
push_back(const Weighted_point &p)
{
    return insert(p);
}

template < class Gt, class Tds >
typename Regular_triangulation_2<Gt,Tds>::Vertex_handle
Regular_triangulation_2<Gt,Tds>::
insert(const Weighted_point &p, Face_handle start)
{
  Locate_type lt;
  int li;
  Face_handle loc = locate(p, lt, li, start);
  return insert(p, lt, loc, li);
}

template < class Gt, class Tds >
typename Regular_triangulation_2<Gt,Tds>::Vertex_handle
Regular_triangulation_2<Gt,Tds>::
insert(const Weighted_point &p, Locate_type lt, Face_handle loc, int li)
{
  if (number_of_vertices() <= 1) return Base::insert(p);
  Vertex_handle v(NULL);
  Oriented_side os;
  switch (lt) {
  case Base::VERTEX:
    if (power_test(loc->vertex(li)->point(), p) != ON_POSITIVE_SIDE)
          return hide_new_vertex(loc,p);
    hide_new_vertex(loc, loc->vertex(li)->point());
    loc->vertex(li)->set_point(p);
    v = loc->vertex(li);
    break;
  case Base::EDGE:
    os = dimension() == 1 ?  power_test(loc,li,p) :
                             power_test(loc,p);
    if (os == ON_NEGATIVE_SIDE)  { //hide preferably in finite face
      if (is_infinite(loc)) loc = loc->neighbor(li);
      return hide_new_vertex(loc,p);
    }
    v = insert_in_edge(p,loc,li);
    break;
  case Base::FACE:
    if (power_test(loc,p) == ON_NEGATIVE_SIDE)
      return hide_new_vertex(loc,p);
    v = insert_in_face(p,loc);
    break;
  case Base::OUTSIDE_CONVEX_HULL:
    v = insert_outside_convex_hull(p,loc);
    break;
  case Base::OUTSIDE_AFFINE_HULL:
    v =  insert_outside_affine_hull(p);
    //clear vertex list of infinite faces which have been copied
     for ( All_faces_iterator afi = all_faces_begin();
	                    afi != all_faces_end(); afi++) {
      if(is_infinite(afi)) afi->vertex_list().clear();
    }
    break;
  default:
    CGAL_triangulation_assertion_msg(false, "locate step failed");
  }
  regularize(v);
  return v;
}


/*
The reinsert function  insert a weighted point which was in a hidden
vertex.
The new and old vertices are then exchanged ; this is required
if the regular triangulation is used with a hierarchy because
the old vertex has its up and down pointers set and other vertices
pointing on him
*/
template < class Gt, class Tds >
typename Regular_triangulation_2<Gt,Tds>::Vertex_handle
Regular_triangulation_2<Gt,Tds>::
reinsert(Vertex_handle v, Face_handle start)
{
  CGAL_triangulation_assertion(v->is_hidden());
  v->set_hidden(false);
  _hidden_vertices--;

  Vertex_handle vh = insert(v->point(), start);
  if(vh->is_hidden()) exchange_hidden(v,vh);
  else  exchange_incidences(v,vh);
  this->_tds.delete_vertex(vh);
  return v;
}


//push va instead of vb in the list of the face fb hiding vb
// vb must be the last inserted vertex in the list of fb
template < class Gt, class Tds >
void
Regular_triangulation_2<Gt,Tds>::
exchange_hidden(Vertex_handle va, Vertex_handle vb)
{
  CGAL_triangulation_assertion (vb->is_hidden());
  CGAL_triangulation_assertion (vb == vb->face()->vertex_list().back());
  vb->face()->vertex_list().pop_back();
  _hidden_vertices--;
  hide_vertex(vb->face(), va);
}

// set to va the incidences of vb
template < class Gt, class Tds >
void
Regular_triangulation_2<Gt,Tds>::
exchange_incidences(Vertex_handle va, Vertex_handle vb)
{
  CGAL_triangulation_assertion ( !vb->is_hidden());
  std::list<Face_handle> faces;
  if (dimension() == 1) {
    faces.push_back(vb->face());
    int i = vb->face()->index(vb);
    faces.push_back(vb->face()->neighbor(1-i));
  }
  else {
    CGAL_triangulation_assertion (dimension() == 2);
    Face_circulator fc = vb->incident_faces(), done(fc);
    do {
      faces.push_back(fc);
      fc++;
    }while(fc != done);
  }

  va->set_face(*(faces.begin()));
  for(typename std::list<Face_handle>::iterator it = faces.begin();
      it != faces.end(); it++){
    Face_handle fh = *it;
    fh->set_vertex(fh->index(vb), va);
  }
  return;
}

template < class Gt, class Tds >
typename Regular_triangulation_2<Gt,Tds>::Vertex_handle
Regular_triangulation_2<Gt,Tds>::
insert_in_face(const Weighted_point &p, Face_handle f)
{
  Vertex_handle v = Base::insert_in_face(p,f);
  update_hidden_points_1_3(f,
			   f->neighbor(ccw(f->index(v))),
			   f->neighbor( cw(f->index(v))) );
  return v;
}

template < class Gt, class Tds >
typename Regular_triangulation_2<Gt,Tds>::Vertex_handle
Regular_triangulation_2<Gt,Tds>::
insert_in_edge(const Weighted_point &p, Face_handle f, int i)
{
  Vertex_handle w = f->vertex(i); //to recignize faces after insertion
  Vertex_handle v = Base::insert_in_edge(p,f,i);
  if (dimension() == 1) {
    Face_handle g = f->neighbor(1 - f->index(v));
    update_hidden_points_2_2(f,g);
  }
  else { //dimension()==2
//     Face_handle g = (v==f->vertex(cw(i))) ? f->neighbor(ccw(i))
//       : f->neighbor( cw(i));
//     update_hidden_points_2_2(f,g);
//     update_hidden_points_2_2(f->neighbor(i), g->neighbor(i));
    int ivf = f->index(v);
    Face_handle g = f->neighbor(cw(ivf))->has_vertex(w) ?
      f->neighbor(cw(ivf)) : f->neighbor(ccw(ivf));
    Face_handle fn = f->neighbor( 3 - f->index(v) - f->index(g));
    Face_handle gn = g->neighbor( 3 - g->index(v) - g->index(w));
    update_hidden_points_2_2(f,fn);
    update_hidden_points_2_2(g,gn);
  }
  return v;
}

template < class Gt, class Tds >
void
Regular_triangulation_2<Gt,Tds>::
regularize(Vertex_handle v)
{
  CGAL_triangulation_precondition( v != infinite_vertex());
  Faces_around_stack faces_around;

  //initialise faces_around
  if (dimension() == 1) {
    faces_around.push_back(v->face());
    faces_around.push_back(v->face()->neighbor(1- v->face()->index(v)));
  }
  else{ //dimension==2
    Face_circulator fit = v->incident_faces(), done(fit);
    do {
      faces_around.push_back(fit++);
    } while(fit != done);
  }

  while( ! faces_around.empty() )
    stack_flip(v, faces_around);
  return;
}


template < class Gt, class Tds >
void
Regular_triangulation_2<Gt,Tds>::
flip(Face_handle f, int i)
{
  Face_handle n = f->neighbor(i);
  Triangulation_2<Gt,Tds>::flip(f,i);
  update_hidden_points_2_2(f,n);
}


template < class Gt, class Tds >
void
Regular_triangulation_2<Gt,Tds>::
remove_degree_3(Vertex_handle v, Face_handle f)
{
  if (f == Face_handle(NULL))    f=v->face();
  update_hidden_points_3_1(f, f->neighbor( cw(f->index(v))),
			   f->neighbor(ccw(f->index(v))));
  Triangulation_2<Gt,Tds>::remove_degree_3(v,f);
  if (is_infinite(f)) { //the list of f is given to its finite neighbor
    Face_handle fn = f->neighbor(f->index(infinite_vertex()));
    set_face(f->vertex_list(),fn);
    fn->vertex_list().splice(fn->vertex_list().begin(),f->vertex_list());
  }

}


template < class Gt, class Tds >
void
Regular_triangulation_2<Gt,Tds>::
remove_hidden(Vertex_handle v )
{
  _hidden_vertices--;
  v->face()->vertex_list().remove(v);
  delete_vertex(v);
  return;
}

template < class Gt, class Tds >
void
Regular_triangulation_2<Gt,Tds>::
remove(Vertex_handle v )
{
  CGAL_triangulation_precondition( v != NULL );
  CGAL_triangulation_precondition(!is_infinite(v));

  if (v->is_hidden()) {
    remove_hidden(v);
    return;
  }

  // remove the hidden vertices before removing the before last vertex
  if(number_of_vertices() == 2 ) {
    Hidden_vertices_iterator hit = hidden_vertices_begin();
    for( ; hit != hidden_vertices_end(); hit = hidden_vertices_begin())
      remove_hidden(hit);
  }

  // As we want to reinsert close to where the point we remove
  // is we take a neighbor face
  Face_handle neighboring = v->face()->neighbor(v->face()->index(v));

  // Collect in p_list
  // the points hidden by the face to be deleted
  Vertex_list p_list;
  if (dimension() == 1) {
    Face_handle f = v->face();
    Face_handle n = f->neighbor(1 - f->index(v));
    p_list.splice(p_list.begin(), f->vertex_list());
    p_list.splice(p_list.begin(), n->vertex_list());
  }
  else if (dimension() == 2 ) {
    Face_circulator fc = v->incident_faces(),done(fc);
    do {
      p_list.splice(p_list.begin(), fc->vertex_list());
      fc++;
    }
    while( fc != done);
  }

  if (dimension() <= 1) {
    Base::remove(v);
    neighboring = infinite_vertex()->face();
  } else {
    remove_2D(v);
  }


  while (! p_list.empty())
  {
    Vertex_handle v(p_list.front());
    p_list.pop_front();
    if (is_infinite(neighboring)) {
      neighboring=neighboring->neighbor(neighboring->index(infinite_vertex()));
    }
    reinsert(v, neighboring);
    neighboring = v->face();
  }
}

template < class Gt, class Tds >
void
Regular_triangulation_2<Gt,Tds>::
remove_2D(Vertex_handle v)
{
  if (test_dim_down(v)) {  this->_tds.remove_dim_down(v);  }
  else {
    std::list<Edge> hole;
    make_hole(v, hole);
    fill_hole_regular(hole);
    delete_vertex(v);
  }
  return;
}


template < class Gt, class Tds >
void
Regular_triangulation_2<Gt,Tds>::
fill_hole_regular(std::list<Edge> & first_hole)
{
  typedef std::list<Edge> Hole;
  typedef std::list<Hole> Hole_list;

  Hole hole;
  Hole_list hole_list;
  Face_handle ff, fn;
  int i, ii, in;

  hole_list.push_front(first_hole);

  while (! hole_list.empty())
    {
      hole = hole_list.front();
      hole_list.pop_front();
      typename Hole::iterator hit = hole.begin();

      // if the hole has only three edges, create the triangle
      if (hole.size() == 3)
	{
	  Face_handle  newf = create_face();
	  hit = hole.begin();
	  for(int j=0; j<3; j++)
	    {
	      ff = (*hit).first;
	      ii = (*hit).second;
	      hit++;
	      ff->set_neighbor(ii,newf);
	      newf->set_neighbor(j,ff);
	      newf->set_vertex(newf->ccw(j),ff->vertex(ff->cw(ii)));
	    }
	  continue;
	}

      // else find an edge with two finite vertices
      // on the hole boundary
      // and the new triangle adjacent to that edge
      //  cut the hole and push it back

      // first, ensure that a neighboring face
      // whose vertices on the hole boundary are finite
      // is the first of the hole
      bool finite = false;
      while (!finite)
	{
	  ff = hole.front().first;
	  ii = hole.front().second;
	  if ( is_infinite(ff->vertex(cw(ii))) ||
	       is_infinite(ff->vertex(ccw(ii))))
	    {
	      hole.push_back(hole.front());
	      hole.pop_front();
	    }
	  else
	    finite = true;
	}

      // take the first neighboring face and pop it;
      ff = hole.front().first;
      ii = hole.front().second;
      hole.pop_front();

      Vertex_handle  v0 = ff->vertex(ff->cw(ii));
      const Weighted_point& p0 = v0->point();
      Vertex_handle  v1 = ff->vertex(ff->ccw(ii));
      const Weighted_point& p1 = v1->point();
      Vertex_handle  v2 = infinite_vertex();
      Weighted_point p2;
      Vertex_handle  vv;
      Weighted_point p;

      typename Hole::iterator hdone = hole.end();
      hit = hole.begin();
      typename Hole::iterator cut_after(hit);

      // if tested vertex is c with respect to the vertex opposite
      // to NULL neighbor,
      // stop at the before last face;
      hdone--;
      while (hit != hdone)
	{
	  fn = (*hit).first;
	  in = (*hit).second;
	  vv = fn->vertex(ccw(in));
	  if (is_infinite(vv))
	    {
	      if (is_infinite(v2))
		cut_after = hit;
	    }
	  else
	    {	// vv is a finite vertex
	      p = vv->point();
	      if (orientation(p0,p1,p) ==
		  COUNTERCLOCKWISE)
		{
		  if (is_infinite(v2))
		    {
		      v2=vv;
		      p2=p;
		      cut_after=hit;
		    }
		  else if (power_test(p0,p1,p2,p) ==
			   ON_POSITIVE_SIDE)
		    {
		      v2=vv;
		      p2=p;
		      cut_after=hit;
		    }
		}
	    }
	  ++hit;
	}

      // create new triangle and update adjacency relations
      Face_handle newf = create_face(v0,v1,v2);
      newf->set_neighbor(2,ff);
      ff->set_neighbor(ii, newf);

      //update the hole and push back in the Hole_List stack
      // if v2 belongs to the neighbor following or preceding *f
      // the hole remain a single hole
      // otherwise it is split in two holes

      fn = hole.front().first;
      in = hole.front().second;
      if (fn->has_vertex(v2, i) && i == (int)fn->ccw(in))
	{
	  newf->set_neighbor(0,fn);
	  fn->set_neighbor(in,newf);
	  hole.pop_front();
	  hole.push_front(Edge(newf,1));
	  hole_list.push_front(hole);
	}
      else
	{
	  fn = hole.back().first;
	  in = hole.back().second;
	  if (fn->has_vertex(v2, i) && i == (int)fn->cw(in))
	    {
	      newf->set_neighbor(1,fn);
	      fn->set_neighbor(in,newf);
	      hole.pop_back();
	      hole.push_back(Edge(newf,0));
	      hole_list.push_front(hole);
	    }
	  else
	    { // split the hole in two holes
	      Hole new_hole;
	      ++cut_after;
	      while (hole.begin() != cut_after)
		{
		  new_hole.push_back(hole.front());
		  hole.pop_front();
		}

	      hole.push_front(Edge(newf,1));
	      new_hole.push_front(Edge(newf,0));
	      hole_list.push_front(hole);
	      hole_list.push_front(new_hole);
	    }
	}
    }
}


template < class Gt, class Tds >
void
Regular_triangulation_2<Gt,Tds>::
set_face(Vertex_list& vl, const Face_handle& fh)
{
  for(typename Vertex_list::iterator it = vl.begin(); it != vl.end(); it++)
    (*it)->set_face(fh);
}

// add the vertex_list of f2 and f3 to the point list of f1
// for the 3-1 flip
template < class Gt, class Tds >
void
Regular_triangulation_2<Gt,Tds>::
update_hidden_points_3_1(const Face_handle& f1, const Face_handle& f2,
			 const Face_handle& f3)
{
  set_face(f2->vertex_list(), f1);
  set_face(f3->vertex_list(), f1);
  (f1->vertex_list()).splice(f1->vertex_list().begin(),f2->vertex_list());
  (f1->vertex_list()).splice(f1->vertex_list().begin(),f3->vertex_list());
  return;
}


// the points of the lists of 2 faces are sorted
// because of a 2-2 flip
template < class Gt, class Tds >
void
Regular_triangulation_2<Gt,Tds>::
update_hidden_points_2_2(const Face_handle& f1, const Face_handle& f2)
{
  CGAL_triangulation_assertion(f1->has_neighbor(f2));

  Vertex_list p_list;
  p_list.splice(p_list.begin(),f1->vertex_list());
  p_list.splice(p_list.begin(),f2->vertex_list());

  // if one of the face is infinite,
  // the other face hide all the points
  if ( is_infinite(f1)) {
    set_face(p_list, f2);
    (f2->vertex_list()).splice(f2->vertex_list().begin(),p_list);
    return;
  }
  if ( is_infinite(f2)) {
    set_face(p_list, f1);
    (f1->vertex_list()).splice(f1->vertex_list().begin(),p_list);
    return;
  }

  if (dimension() == 1) {
    const Weighted_point& a1 = f1->vertex(f1->index(f2))->point();
    const Weighted_point& a  = f1->vertex(1-f1->index(f2))->point();
    while ( ! p_list.empty() ) {
      if ( compare_x(a, p_list.front()->point()) ==
	   compare_x(a, a1)  &&
	   compare_y(a, p_list.front()->point()) ==
	   compare_y(a, a1))
	{
	  hide_vertex(f1, p_list.front());
	} else {
	hide_vertex(f2, p_list.front());
	}
      p_list.pop_front();
    }
    return;
  }

  // from here f1 and f2 are finite 2-dimensional faces
  int idx2 = f1->index(f2);
  Vertex_handle v0=f1->vertex(ccw(idx2));
  Vertex_handle v1=f1->vertex(cw(idx2));
  CGAL_triangulation_assertion( !is_infinite(v0) && !is_infinite(v1));

  while ( ! p_list.empty() )
    {
      if (orientation(v0->point(), v1->point(), p_list.front()->point()) ==
	  COUNTERCLOCKWISE)
	hide_vertex(f1, p_list.front());
	else
	hide_vertex(f2, p_list.front());
      p_list.pop_front();
    }
}

// The point list of f1 is separated into 3 lists
// for a 1-3 flip
template < class Gt, class Tds >
void
Regular_triangulation_2<Gt,Tds>::
update_hidden_points_1_3(const Face_handle& f1, const Face_handle& f2,
			 const Face_handle& f3)
{
    CGAL_triangulation_assertion(f1->has_neighbor(f2) &&
			      f2->has_neighbor(f3) &&
			      f3->has_neighbor(f1));


    Vertex_list p_list;
    p_list.splice(p_list.begin(),f1->vertex_list());
    if (p_list.empty())
	return;

    // the following does not work if
    // two of f1,f2 and f3 are twice neighbors
    // but this cannot appear taking the assertion into account;
    int idx2 = f1->index(f2),
        idx3 = f1->index(f3);
    Vertex_handle v2 = f1->vertex(idx2),
                  v3 = f1->vertex(idx3),
                  v0 = f1->vertex(3-(idx2+idx3)),
                  v1 = f2->vertex(f2->index(f1));

    CGAL_triangulation_assertion(f2->has_vertex(v0) && f1->has_vertex(v0));
    CGAL_triangulation_assertion(f3->has_vertex(v1));
    CGAL_triangulation_assertion( ! is_infinite(v0));

    // if two of f1, f2,and f3 are infinite
    // the list goes entirely to the third finite face
    // no orientation test necessary
    // because the point list of an infinite face
    // is only made of point projecting on its finite edge
    if ( is_infinite(f1 ) && is_infinite(f2)) {
      set_face(p_list, f3);
      f3->vertex_list().splice(f3->vertex_list().begin(), p_list);
      return;
    }
    if ( is_infinite(f1) && is_infinite(f3)) {
      set_face(p_list, f2);
      f2->vertex_list().splice(f2->vertex_list().begin(), p_list);
      return;
    }
    if ( is_infinite(f2) && is_infinite(f3)){
      set_face(p_list, f1);
      f1->vertex_list().splice(f1->vertex_list().begin(), p_list);
      return;
    }

    // if here, v1,v2,v3 and v0 are finite vertices
    while(! p_list.empty())
    {
      Vertex_handle v(p_list.front());
//       if(orientation(v2->point(),v0->point(), v->point()) !=
// 	 orientation(v2->point(),v0->point(),v3->point()) )
//       { // not in f1
// 	if (orientation(v1->point(), v0->point(), v->point() ) !=
// 	    orientation(v1->point(), v0->point(), v3->point() ) )
// 	  // not in f2
// 	    hide_vertex(f3, v);
// 	   else
// 	    hide_vertex(f2, v);
//       }
//       else
// 	  hide_vertex(f1, v);
      if(orientation(v2->point(),v0->point(), v->point()) ==
 	 orientation(v2->point(),v0->point(),v3->point())  &&
	 orientation(v3->point(),v0->point(), v->point()) ==
	 orientation(v3->point(),v0->point(), v2->point()))
	hide_vertex(f1, v);
      else if (orientation(v1->point(), v0->point(), v->point()) ==
	       orientation(v1->point(), v0->point(), v3->point()) )
	hide_vertex(f2,v);
      else hide_vertex(f3,v);
      p_list.pop_front();
    }
}

// the vertex is a degree three vertex which has to removed
// and hidden
// create first  a new hidden vertex and exchange with the vertex
// to be removed by the tds :
// this is required to keep up and down pointers right when using a hierarchy
template < class Gt, class Tds >
void
Regular_triangulation_2<Gt,Tds>::
hide_remove_degree_3(Face_handle fh, Vertex_handle vh)
{
 Vertex_handle vnew= this->_tds.create_vertex();
 exchange_incidences(vnew,vh);
 remove_degree_3(vnew, fh);
 hide_vertex(fh,vh);
}

// create a vertex and hide it
template < class Gt, class Tds >
typename Regular_triangulation_2<Gt,Tds>::Vertex_handle
Regular_triangulation_2<Gt,Tds>::
hide_new_vertex(Face_handle f, const Weighted_point& p)
{
  Vertex_handle v = this->_tds.create_vertex();
  v->set_point(p);
  hide_vertex(f, v);
  return v;
}

// insert the vertex to the hidden vertex list
template < class Gt, class Tds >
void
Regular_triangulation_2<Gt,Tds>::
hide_vertex(Face_handle f, Vertex_handle vh)
{
  // no hidden vertex in infinite face
  if(is_infinite(f)) f = f->neighbor(f->index(infinite_vertex()));

  if(! vh->is_hidden()) {
    vh->set_hidden(true);
    _hidden_vertices++;
  }
  vh->set_face(f);
  f->vertex_list().push_back(vh);
}

// template < class Gt, class Tds >
// void
// Regular_triangulation_2<Gt,Tds>::
// hide_vertex(Face_handle f, void* ptr)
// {
//   Vertex_handle v(static_cast<Vertex*>(ptr));
//   hide_vertex(f, v);
// }



template < class Gt, class Tds >
void
Regular_triangulation_2<Gt,Tds>::
stack_flip(Vertex_handle v, Faces_around_stack &faces_around)
{
  Face_handle f=faces_around.front();
  faces_around.pop_front();
  int i = f->index(v);
  Face_handle n = f->neighbor(i);

  if (dimension() == 1 ) {
    if ( is_infinite(f)  || is_infinite(n) ) return;
    if ( power_test( v->point(),
		     n->vertex(n->index(f))->point(),
		     f->vertex(1-i)->point()) ==  ON_NEGATIVE_SIDE)
      stack_flip_dim1(f,i,faces_around);
    return;
  }

  // now dimension() == 2
  //test the regularity of edge (f,i)
  //if( power_test(n, v->point()) == ON_NEGATIVE_SIDE)
  if( power_test(n, v->point()) != ON_POSITIVE_SIDE)
    return;

  if(is_infinite(f,i))
    {
      int j = 3 - ( i + f->index(infinite_vertex()));
      if ( f->vertex(j)->degree() == 4)
	stack_flip_4_2(f,i,j,faces_around);
      return;
    }

    // now f and n are both finite faces
    int ni = n->index(f);
    Orientation occw = orientation(f->vertex(i)->point(),
				   f->vertex(ccw(i))->point(),
				   n->vertex(ni)->point());
    Orientation ocw  = orientation(f->vertex(i)->point(),
				   f->vertex(cw(i))->point(),
				   n->vertex(ni)->point());
    if (occw == LEFT_TURN && ocw == RIGHT_TURN) {
      // quadrilater (f,n) is convex
      stack_flip_2_2(f,i, faces_around);
      return;
    }
    if (occw == RIGHT_TURN && f->vertex(ccw(i))->degree() == 3) {
      stack_flip_3_1(f,i,ccw(i),faces_around);
      return;
    }
    if (ocw == LEFT_TURN && f->vertex(cw(i))->degree() == 3) {
      stack_flip_3_1(f,i,cw(i),faces_around);
      return;
    }
    if (occw == COLLINEAR && f->vertex(ccw(i))->degree() == 4) {
      stack_flip_4_2(f,i,ccw(i),faces_around);
      return;
    }
    if (ocw == COLLINEAR && f->vertex(cw(i))->degree() == 4)
      stack_flip_4_2(f,i,cw(i),faces_around);

    return;
}


template < class Gt, class Tds >
void
Regular_triangulation_2<Gt,Tds>::
stack_flip_4_2(Face_handle f, int i, int j, Faces_around_stack & faces_around)
{
    int k = 3-(i+j);
    Face_handle g=f->neighbor(k);
    if (!faces_around.empty())
    {
      if (faces_around.front() == g)
	  faces_around.pop_front();
      else if (faces_around.back() == g)
	  faces_around.pop_back();
    }

    //union f with  g and f->neihgbor(i) with g->f->neihgbor(i)
    Face_handle fn = f->neighbor(i);
    //Face_handle gn = g->neighbor(g->index(f->vertex(i)));
    Vertex_handle vq = f->vertex(j);

    this->_tds.flip( f, i); //not using flip because the vertex j is flat.
    update_hidden_points_2_2(f,fn);
    Face_handle h1 = ( f->has_vertex(vq) ? fn : f);
    //hide_vertex(h1, vq);
    hide_remove_degree_3(g,vq);
    faces_around.push_front(g);
    faces_around.push_front(h1);
}


template < class Gt, class Tds >
void
Regular_triangulation_2<Gt,Tds>::
stack_flip_3_1(Face_handle f, int i, int j, Faces_around_stack & faces_around)
{
  int k = 3-(i+j);
  Face_handle g=f->neighbor(k);
  if (!faces_around.empty())
  {
    if (faces_around.front()== g)
	faces_around.pop_front();
    else if ( faces_around.back() == g)
	faces_around.pop_back();
  }

  Vertex_handle vq= f->vertex(j);
  //hide_vertex(f,vq);
  hide_remove_degree_3(f,vq);
  faces_around.push_front(f);
}


template < class Gt, class Tds >
void
Regular_triangulation_2<Gt,Tds>::
stack_flip_2_2(Face_handle f, int i, Faces_around_stack & faces_around)
{
    Vertex_handle vq = f->vertex(ccw(i));
    flip(f,i);
    if(f->has_vertex(vq)) {
      faces_around.push_front(f->neighbor(ccw(i)));
      faces_around.push_front(f);
    }
    else {
      faces_around.push_front(f);
      faces_around.push_front(f->neighbor(cw(i)));
    }
}

template < class Gt, class Tds >
void
Regular_triangulation_2<Gt,Tds>::
stack_flip_dim1(Face_handle f, int i, Faces_around_stack &faces_around)
{
  Vertex_handle va = f->vertex(1-i);
  Face_handle n= f->neighbor(i);
  int in = n->index(f);
  Vertex_handle vb = n->vertex(in);
  f->set_vertex(1-i, n->vertex(in));
  vb->set_face(f);
  f->set_neighbor(i, n->neighbor(1-in));
  n->neighbor(1-in)->set_neighbor(n->neighbor(1-in)->index(n), f);
  (f->vertex_list()).splice(f->vertex_list().begin(),n->vertex_list());
  set_face(f->vertex_list(),f);
  delete_face(n);
  hide_vertex(f,va);
  faces_around.push_front(f);
  return;
}


template < class Gt, class Tds >
typename Regular_triangulation_2<Gt,Tds>::All_vertices_iterator
Regular_triangulation_2<Gt,Tds>::
all_vertices_begin () const
{
  return filter_iterator(Base::all_vertices_begin(),
			 Base::all_vertices_end(),
			 Hidden_tester());
}

template < class Gt, class Tds >
typename Regular_triangulation_2<Gt,Tds>::All_vertices_iterator
Regular_triangulation_2<Gt,Tds>::
all_vertices_end () const
{
  return filter_iterator(Base::all_vertices_begin(),
			 Base::all_vertices_end(),
			 Hidden_tester(),
			 Base::all_vertices_end());
}

template < class Gt, class Tds >
typename Regular_triangulation_2<Gt,Tds>::Finite_vertices_iterator
Regular_triangulation_2<Gt,Tds>::
finite_vertices_begin () const
{
  return filter_iterator(Base::finite_vertices_begin(),
			 Base::finite_vertices_end(),
			 Hidden_tester());
}

template < class Gt, class Tds >
typename Regular_triangulation_2<Gt,Tds>::Finite_vertices_iterator
Regular_triangulation_2<Gt,Tds>::
finite_vertices_end () const
{

  return filter_iterator(Base::finite_vertices_begin(),
			 Base::finite_vertices_end(),
			 Hidden_tester(),
			 Base::finite_vertices_end());

}

template < class Gt, class Tds >
typename Regular_triangulation_2<Gt,Tds>::Hidden_vertices_iterator
Regular_triangulation_2<Gt,Tds>::
hidden_vertices_begin () const
{
  return filter_iterator(Base::finite_vertices_begin(),
			 Base::finite_vertices_end(),
			 Unhidden_tester());

}

template < class Gt, class Tds >
typename Regular_triangulation_2<Gt,Tds>::Hidden_vertices_iterator
Regular_triangulation_2<Gt,Tds>::
hidden_vertices_end () const
{
  return filter_iterator(Base::finite_vertices_begin(),
			 Base::finite_vertices_end(),
			 Unhidden_tester(),
			 Base::finite_vertices_end());

}

CGAL_END_NAMESPACE

#endif // CGAL_REGULAR_TRIANGULATION_2_H