#pragma once

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/convex_hull_3.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel INEXACT_K;
typedef CGAL::Polyhedron_3<INEXACT_K> Polyhedron_3;

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/intersections.h>
typedef CGAL::Exact_predicates_exact_constructions_kernel EXACT_K;
