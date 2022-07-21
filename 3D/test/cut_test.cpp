#include "3D/Facet.h"
#include "3D/Plane.h"
#include "3D/Transformation.h"
#include <GUnit.h>

GTEST("Dummy") {
    SHOULD("Recognize True") {
        EXPECT(true);
    }
}

GTEST("Cut") {
    Plane p{ 1., 0., 0., 0. };
    Facet f {
        .normal{0., 0., 1.},
        .vertices{ {1., 0., 0.}, {-1., 1., 0.}, {-1., -1., 0.} }
    };

    SHOULD("Drop facet out of half-plane") {
        TransformV translateV = [](Vertex v) { v.x += 2.; return v;};
        Facet f_pos = applyTransform(translateV, f);
        std::vector<Facet> result = p.cut(f_pos);
        ASSERT_EQ(result.size(), 0);
    }
    SHOULD("Keep facet in half-plane") {
        TransformV translateV = [](Vertex v) { v.x -= 2.; return v;};
        Facet f_pos = applyTransform(translateV, f);
        std::vector<Facet> result = p.cut(f_pos);
        ASSERT_EQ(result.size(), 1);
        EXPECT_EQ(result[0], f_pos);
    }
    SHOULD("Cut facet with 1 vertex in half-plane") {
        TransformV translateV = [](Vertex v) { v.x =- v.x; return v;};
        Facet f_pos = applyTransform(translateV, f);
        std::vector<Facet> result = p.cut(f_pos);
        ASSERT_EQ(result.size(), 1);
        const Facet& facet =  result[0];
        EXPECT_EQ(facet.normal, f_pos.normal);
        EXPECT_TRUE(p.isIn(facet.vertices[0]));
        EXPECT_TRUE(p.isOnBorder(facet.vertices[1]));
        EXPECT_TRUE(p.isOnBorder(facet.vertices[2]));
        EXPECT_TRUE(p.isOnBorder(facet.vertices[2]));

        EXPECT_TRUE(facet.isParallel(f));
        EXPECT_TRUE(facet.hasSameSide(f_pos));
    }
    SHOULD("Split facet with 2 vertex in half-plane") {
        std::vector<Facet> result = p.cut(f);
        ASSERT_EQ(result.size(), 2);
        {
            const Facet& facet =  result[0];
            EXPECT_EQ(facet.normal, f.normal);
            EXPECT_TRUE(p.isIn(facet));
            EXPECT_TRUE(facet.isParallel(f));
            EXPECT_TRUE(facet.hasSameSide(f));
        }
        {
            const Facet& facet =  result[1];
            EXPECT_EQ(facet.normal, f.normal);
            EXPECT_TRUE(p.isIn(facet));
            EXPECT_TRUE(facet.isParallel(f));
            EXPECT_TRUE(facet.hasSameSide(f));
        }
    }
}
