#include <iostream>
#include <physics.h>
#include <gtest/gtest.h>
#include <glm/glm.hpp>
#include <math5d.h>

struct PoincareParams
{
    glm::vec3 linAcceleration;
    glm::vec3 angAcceleration;
    glm::vec4 translation;

    friend std::ostream& operator<<(std::ostream& os, const PoincareParams& p)
    {
        os << "[vec3(" << p.linAcceleration.x << ", " << p.linAcceleration.y << ", " << p.linAcceleration.z << "), "
           << "[vec3(" << p.angAcceleration.x << ", " << p.angAcceleration.y << ", " << p.angAcceleration.z << "), "
           << "vec4(" << p.translation.x << ", " << p.translation.y << ", " << p.translation.z << ", " << p.translation.w << "), ";
        return os;
    }
};

class PhysicsPoincareTest : public ::testing::TestWithParam<PoincareParams> {
protected:
    Physics physics_obj;
    PhysicsPoincareTest() : physics_obj(0.01f) {}
};

Mat5 analyticPoincareMat(float time, const glm::vec3& linAcceleration, const glm::vec3& angAcceleration, const glm::vec4& translation)
{
    Mat5 analytic(0.0f);
    analytic[0] = Vec5(0.0f, linAcceleration[0], linAcceleration[1], linAcceleration[2], 0.0f); 
    analytic[1] = Vec5(linAcceleration[0], 0.0f, -angAcceleration[2], angAcceleration[1], 0.0f); 
    analytic[2] = Vec5(linAcceleration[1], angAcceleration[2], 0.0f, -angAcceleration[0], 0.0f);
    analytic[3] = Vec5(linAcceleration[2], -angAcceleration[1], angAcceleration[0], 0.0f, 0.0f);
    analytic[4] = Vec5(translation[0], translation[1], translation[2], translation[3], 0.0f);
    analytic = analytic * time;
    analytic = analytic.exp();
    return analytic;
}


TEST_P(PhysicsPoincareTest, PoincareValidation)
{
    float tolerance = 0.001f;
    PoincareParams params = GetParam();
    Vec5 position(0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    Vec5 analyticResult(0.0f);
    Vec5 numericalResult(0.0f);
    Mat5 analyticPoincare(0.0f);
    Mat5 numericalPoincare(0.0f);
    physics_obj.buildPoincareGenerator(params.linAcceleration, params.angAcceleration, params.translation);

    for (auto i = 0; i < 100; i++)
    {
        // teste dos invariantes
        EXPECT_FALSE(std::isnan(physics_obj.getVelocityMagnitude())) << "velocidade retornou NaN no passo: " << i << std::endl;
        EXPECT_FALSE(std::isnan(physics_obj.getGamma())) << "gamma retornou NaN no passo: " << i << std::endl;
        EXPECT_LT(physics_obj.getVelocityMagnitude(), 1.0f + tolerance) << "velocidade ultrapassou o limite no passo: " << i << std::endl;

        // teste das matrizes
        numericalPoincare = physics_obj.getPoincareMat();
        analyticPoincare = analyticPoincareMat(physics_obj.getProperTime(), params.linAcceleration, params.angAcceleration, params.translation);
        numericalResult = numericalPoincare * position;
        analyticResult = analyticPoincare * position;
        EXPECT_TRUE(math5::compare(numericalResult, analyticResult, tolerance)) << "vetor analitico e numerico divergiram no passo: " << i << std::endl;

        physics_obj.update();
    }
}

INSTANTIATE_TEST_SUITE_P(
    CenariosEscolhidos,
    PhysicsPoincareTest,
    ::testing::Values(

        PoincareParams{ glm::vec3(0.0f), glm::vec3(0.0f), glm::vec4(0.0f)},

        PoincareParams{ glm::vec3(1, 0, 0), glm::vec3(0.0f), glm::vec4(0.0f)},

        PoincareParams{ glm::vec3(10, 0, 0), glm::vec3(0.0f), glm::vec4(0.0f)},

        PoincareParams{ glm::vec3(20, 0, 0), glm::vec3(0.0f), glm::vec4(0.0f)},

        PoincareParams{ glm::vec3(30, 0, 0), glm::vec3(0.0f), glm::vec4(0.0f)},

        PoincareParams{ glm::vec3(40, 0, 0), glm::vec3(0.0f), glm::vec4(0.0f)},

        PoincareParams{ glm::vec3(50, 0, 0), glm::vec3(0.0f), glm::vec4(0.0f)},

        PoincareParams{ glm::vec3(10, 0, 0), glm::vec3(0.0f), glm::vec4(0.01f, 0.0f, 0.0f, 0.0f)},

        PoincareParams{ glm::vec3(10, 0, 0), glm::vec3(0.0f), glm::vec4(0.01f, 0.01f, 0.0f, 0.0f)},

        PoincareParams{ glm::vec3(10, 0, 0), glm::vec3(0.0f), glm::vec4(0.01f, 0.001f, 0.0f, 0.0f)}
    )
);


