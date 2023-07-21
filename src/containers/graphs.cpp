#define CATCH_CONFIG_MAIN
#include <boost/graph/adjacency_list.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <catch2/catch.hpp>
#include <set>

/* std::Adjacency_List */

TEST_CASE("boost::adjacency_list stores graph data") {
    using boost::adjacency_list, boost::add_vertex, boost::add_edge, boost::adjacent_vertices, boost::num_vertices,
        boost::num_edges;

    adjacency_list<> graph{};
    auto vertex1 = add_vertex(graph);
    auto vertex2 = add_vertex(graph);
    auto vertex3 = add_vertex(graph);
    auto vertex4 = add_vertex(graph);

    auto edge1_2 = add_edge(vertex1, vertex2, graph);
    auto edge2_1 = add_edge(vertex2, vertex1, graph);
    auto edge1_3 = add_edge(vertex1, vertex3, graph);
    auto edge3_4 = add_edge(vertex3, vertex4, graph);
    auto edg4_2 = add_edge(vertex4, vertex2, graph);

    REQUIRE(num_vertices(graph) == 4);
    REQUIRE(num_edges(graph) == 5);

    auto [begin, end] = adjacent_vertices(vertex1, graph);
    std::set<decltype(vertex1)> neighbours1{begin, end};
    REQUIRE(neighbours1.count(vertex2) == 1);
    REQUIRE(neighbours1.count(vertex3) == 1);
    REQUIRE(neighbours1.count(vertex4) == 0);
}

/* std::Adjacency_List */

/* boost::Property_Tree::Ptree */

TEST_CASE("boost::property_tree::ptree stores tree data") {
    using namespace boost::property_tree;
    ptree p;
    p.put("name", "finfisher");
    p.put("year", 2014);
    p.put("features.process", "LSASS");
    p.put("features.driver", "mssounddx.sys");
    p.put("features.arch", 32);
    REQUIRE(p.get_child("year").get_value<int>() == 2014);

    const auto fileName = "rootKit.json";
    write_json(fileName, p);

    ptree pCopy;
    read_json(fileName, pCopy);
    REQUIRE(pCopy == p);
}

/* boost::Property_Tree::Ptree */
