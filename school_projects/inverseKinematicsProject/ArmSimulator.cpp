#include <vector>

#include "ArmSimulator.h"

/// <summary>
/// Constructor
/// NOTE you can set the speed here
/// </summary>
/// <param name="name"></param>
/// <param name="link1"></param>
/// <param name="link2"></param>
ArmSimulator::ArmSimulator(const std::string& name, LinkRef link1, LinkRef link2, LinkRef link3)
    : BaseSimulator(name), link1_(link1), link2_(link2), link3_(link3), speed_(0.03), start_(1), end_(0) {
    std::string particle_name = name + "ControlPoint";
    control_point_ = Particle::create(particle_name);
    control_point_->setPosition(vec3(-4.20, 2.0, 3.0));
    control_point_->setColor(vec3(0.36, 0.86, 0.67));




    hand_ = Particle::create(particle_name + "Hand");
    vec3 offSet = vec3(-4.20, 2.0, 3.5) + vec3(0.0, 0.0, 0.5);
    hand_->setPosition(offSet);
    hand_->setColor(vec3(1, 0, 0));

    nextPoint_ = 0;

}


/// <summary>
/// Init
/// </summary>
/// <param name="time">Init time</param>
/// <returns></returns>
int ArmSimulator::init(double time) {
    control_point_->setPosition(start_);
    control_point_->init(time);
    link1_->init(l_one_start_, l_one_end_, 1);
    link2_->init(l_two_start_, l_two_end_, 0);
    link3_->init(l_thr_start_, l_thr_end_, 0);
    origin_ = link1_->getStart();
    min_r_ = abs(link1_->length - link2_->length - link3_->length);
    max_r_ = abs(link1_->length + link2_->length + link3_->length);



    return 0;
}



/// <summary>
/// Return the rotation around z matrix given the rotation theta
/// </summary>
/// <param name="theta"></param>
/// <returns></returns>
mat4 getZRotationMatrix(float theta) {
    return mat4(vec4(cos(theta), -sin(theta), 0, 0), vec4(sin(theta), cos(theta), 0, 0), vec4(0, 0, 1, 0), vec4(0, 0, 0, 1));
}

mat4 getXRotationMatrix(float theta)
{
    return mat4(vec4(1, 0, 0, 0), vec4(0, cos(theta), -sin(theta), 0), vec4(0, sin(theta), cos(theta), 0), vec4(0, 0, 0, 1));
}

mat4 getYRotationMatrix(float theta)
{
    return mat4(
        vec4(cos(theta), 0, sin(theta), 0), vec4(0, 1, 0, 0), vec4(-sin(theta), 0, cos(theta), 0), vec4(0, 0, 0, 1));
}

/* Derivatites of rotation matrices*/
mat4 getXRotationMatrixDev(float theta)
{
    return mat4(vec4(0, 0, 0, 0), vec4(0, -sin(theta), -cos(theta), 0), vec4(0, cos(theta), -sin(theta), 0), vec4(0, 0, 0, 0));
}

mat4 getYRotationMatrixDev(float theta)
{
    return mat4(
        vec4(-sin(theta), 0, cos(theta), 0), vec4(0, 0, 0, 0), vec4(-cos(theta), 0, sin(theta), 0), vec4(0, 0, 0, 0));
}

mat4 getZRotationMatrixDev(float theta)
{
    return mat4(vec4(-sin(theta), -cos(theta), 0, 0), vec4(cos(theta), -sin(theta), 0, 0), vec4(0, 0, 0, 0), vec4(0, 0, 0, 0));
}

/*Translataion Matrix*/
mat4 getTranslationMatrix(float tx, float ty, float tz)
{
    return mat4(vec4(1, 0, 0, 0), vec4(0, 1, 0, 0), vec4(0, 0, 1, 0), vec4(0, 0, 0, 1));
}

/* First Jacobian Column */
vec4 ArmSimulator::getJacobian_colOne()
{

    mat4 sh_rotz = getZRotationMatrix(theta_3_);
    mat4 sh_roty = getYRotationMatrix(theta_2_);
    mat4 dt_sh_rotx = getXRotationMatrixDev(theta_1_);
    /*elbow rotation*/
    mat4 el_roty = getYRotationMatrix(theta_5_);
    mat4 el_rotx = getXRotationMatrix(theta_4_);
    /*wrist rotation*/
    mat4 wr_roty = getYRotationMatrix(theta_7_);
    mat4 wr_rotz = getZRotationMatrix(theta_6_);
    /*shoulder, elbow, and wrist translations*/
    mat4 sh_transl = getTranslationMatrix(1.0f, 1.0f, 1.0f);
    mat4 el_transl = getTranslationMatrix(1.0f, 1.0f, 1.0f);
    mat4 wr_transl = getTranslationMatrix(1.0f, 1.0f, 1.0f);
    mat4 rt_transl = mat4(1.0f);

    // the Jacobian matrix colum product to compute for.
    mat4 jMcolProduct(1.0f);

    // theta one Jacobian column multiplications
    jMcolProduct = rt_transl * sh_transl;
    jMcolProduct = jMcolProduct * sh_rotz;
    jMcolProduct = jMcolProduct * sh_roty;
    jMcolProduct = jMcolProduct * dt_sh_rotx;
    jMcolProduct = jMcolProduct * el_transl;
    jMcolProduct = jMcolProduct * el_roty;
    jMcolProduct = jMcolProduct * el_rotx;
    jMcolProduct = jMcolProduct * wr_transl;
    jMcolProduct = jMcolProduct * wr_roty;
    jMcolProduct = jMcolProduct * wr_rotz;

    vec3 dirct = hand_->getPosition();
    vec4 dir_t = vec4(dirct[0], dirct[1], dirct[2], 1);
    vec4 col_one = jMcolProduct * dir_t;
    return col_one;
}

/* Second Jacobian Column */
vec4 ArmSimulator::getJacobian_colTwo()
{

    mat4 sh_rotz = getZRotationMatrix(theta_3_);
    // derivative on theta_2
    mat4 dt_sh_roty = getYRotationMatrixDev(theta_2_);
    mat4 sh_rotx = getXRotationMatrix(theta_1_);

    /*elbow rotation*/
    mat4 el_roty = getYRotationMatrix(theta_5_);
    mat4 el_rotx = getXRotationMatrix(theta_4_);
    /*wrist rotation*/
    mat4 wr_roty = getYRotationMatrix(theta_7_);
    mat4 wr_rotz = getZRotationMatrix(theta_6_);
    /*shoulder, elbow, and wrist translations*/
    mat4 sh_transl = getTranslationMatrix(1.0f, 1.0f, 1.0f);
    mat4 el_transl = getTranslationMatrix(1.0f, 1.0f, 1.0f);
    mat4 wr_transl = getTranslationMatrix(1.0f, 1.0f, 1.0f);
    mat4 rt_transl = mat4(1.0f);

    // the Jacobian matrix colum product to compute for.
    mat4 jMcolProduct(1.0f);

    // theta two Jacobian column multiplications
    jMcolProduct = rt_transl * sh_transl;
    jMcolProduct = jMcolProduct * sh_rotz;
    jMcolProduct = jMcolProduct * dt_sh_roty;
    jMcolProduct = jMcolProduct * sh_rotx;
    jMcolProduct = jMcolProduct * el_transl;
    jMcolProduct = jMcolProduct * el_roty;
    jMcolProduct = jMcolProduct * el_rotx;
    jMcolProduct = jMcolProduct * wr_transl;
    jMcolProduct = jMcolProduct * wr_roty;
    jMcolProduct = jMcolProduct * wr_rotz;

    vec3 dirct = hand_->getPosition();
    vec4 dir_t = vec4(dirct[0], dirct[1], dirct[2], 1);
    vec4 col_one = jMcolProduct * dir_t;
    return col_one;
}

/* Third Jacobian Column */
vec4 ArmSimulator::getJacobian_colThree()
{

    // derivative on theta_2
    mat4 dt_sh_rotz = getZRotationMatrixDev(theta_3_);
    mat4 sh_roty = getYRotationMatrix(theta_2_);
    mat4 sh_rotx = getXRotationMatrix(theta_1_);

    /*elbow rotation*/
    mat4 el_roty = getYRotationMatrix(theta_5_);
    mat4 el_rotx = getXRotationMatrix(theta_4_);
    /*wrist rotation*/
    mat4 wr_roty = getYRotationMatrix(theta_7_);
    mat4 wr_rotz = getZRotationMatrix(theta_6_);
    /*shoulder, elbow, and wrist translations*/
    mat4 sh_transl = getTranslationMatrix(1.0f, 1.0f, 1.0f);
    mat4 el_transl = getTranslationMatrix(1.0f, 1.0f, 1.0f);
    mat4 wr_transl = getTranslationMatrix(1.0f, 1.0f, 1.0f);
    mat4 rt_transl = mat4(1.0f);

    // the Jacobian matrix colum product to compute for.
    mat4 jMcolProduct(1.0f);


    // theta three Jacobian column multiplications
    jMcolProduct = rt_transl * sh_transl;
    jMcolProduct = jMcolProduct * dt_sh_rotz;
    jMcolProduct = jMcolProduct * sh_roty;
    jMcolProduct = jMcolProduct * sh_rotx;
    jMcolProduct = jMcolProduct * el_transl;
    jMcolProduct = jMcolProduct * el_roty;
    jMcolProduct = jMcolProduct * el_rotx;
    jMcolProduct = jMcolProduct * wr_transl;
    jMcolProduct = jMcolProduct * wr_roty;
    jMcolProduct = jMcolProduct * wr_rotz;

    vec3 dirct = hand_->getPosition();
    vec4 dir_t = vec4(dirct[0], dirct[1], dirct[2], 1);
    vec4 col_one = jMcolProduct * dir_t;
    return col_one;
}

/* Fourth Jacobian Column */
vec4 ArmSimulator::getJacobian_colFour()
{

    mat4 sh_rotz = getZRotationMatrix(theta_3_);
    mat4 sh_roty = getYRotationMatrix(theta_2_);
    mat4 sh_rotx = getXRotationMatrix(theta_1_);

    /*elbow rotation*/
    mat4 el_roty = getYRotationMatrix(theta_5_);
    // derivative on theta_4
    mat4 dt_el_rotx = getXRotationMatrixDev(theta_4_);
    /*wrist rotation*/
    mat4 wr_roty = getYRotationMatrix(theta_7_);
    mat4 wr_rotz = getZRotationMatrix(theta_6_);
    /*shoulder, elbow, and wrist translations*/
    mat4 sh_transl = getTranslationMatrix(1.0f, 1.0f, 1.0f);
    mat4 el_transl = getTranslationMatrix(1.0f, 1.0f, 1.0f);
    mat4 wr_transl = getTranslationMatrix(1.0f, 1.0f, 1.0f);
    mat4 rt_transl = mat4(1.0f);

    // the Jacobian matrix colum product to compute for.
    mat4 jMcolProduct(1.0f);


    // theta four Jacobian column multiplications
    jMcolProduct = rt_transl * sh_transl;
    jMcolProduct = jMcolProduct * sh_rotz;
    jMcolProduct = jMcolProduct * sh_roty;
    jMcolProduct = jMcolProduct * sh_rotx;
    jMcolProduct = jMcolProduct * el_transl;
    jMcolProduct = jMcolProduct * el_roty;
    jMcolProduct = jMcolProduct * dt_el_rotx;
    jMcolProduct = jMcolProduct * wr_transl;
    jMcolProduct = jMcolProduct * wr_roty;
    jMcolProduct = jMcolProduct * wr_rotz;

    vec3 dirct = hand_->getPosition();
    vec4 dir_t = vec4(dirct[0], dirct[1], dirct[2], 1);
    vec4 col_one = jMcolProduct * dir_t;
    return col_one;
}


/* Fifth Jacobian Column */
vec4 ArmSimulator::getJacobian_colFive()
{

    mat4 sh_rotz = getZRotationMatrix(theta_3_);
    mat4 sh_roty = getYRotationMatrix(theta_2_);
    mat4 sh_rotx = getXRotationMatrix(theta_1_);

    /*elbow rotation*/
    // derivative on theta_5
    mat4 dt_el_roty = getYRotationMatrixDev(theta_5_);
    mat4 el_rotx = getXRotationMatrix(theta_4_);
    /*wrist rotation*/
    mat4 wr_roty = getYRotationMatrix(theta_7_);
    mat4 wr_rotz = getZRotationMatrix(theta_6_);
    /*shoulder, elbow, and wrist translations*/
    mat4 sh_transl = getTranslationMatrix(1.0f, 1.0f, 1.0f);
    mat4 el_transl = getTranslationMatrix(1.0f, 1.0f, 1.0f);
    mat4 wr_transl = getTranslationMatrix(1.0f, 1.0f, 1.0f);
    mat4 rt_transl = mat4(1.0f);

    // the Jacobian matrix colum product to compute for.
    mat4 jMcolProduct(1.0f);


    // theta five Jacobian column multiplications
    jMcolProduct = rt_transl * sh_transl;
    jMcolProduct = jMcolProduct * sh_rotz;
    jMcolProduct = jMcolProduct * sh_roty;
    jMcolProduct = jMcolProduct * sh_rotx;
    jMcolProduct = jMcolProduct * el_transl;
    jMcolProduct = jMcolProduct * dt_el_roty;
    jMcolProduct = jMcolProduct * el_rotx;
    jMcolProduct = jMcolProduct * wr_transl;
    jMcolProduct = jMcolProduct * wr_roty;
    jMcolProduct = jMcolProduct * wr_rotz;

    vec3 dirct = hand_->getPosition();
    vec4 dir_t = vec4(dirct[0], dirct[1], dirct[2], 1);
    vec4 col_one = jMcolProduct * dir_t;
    return col_one;
}


/* Sixth Jacobian Column */
vec4 ArmSimulator::getJacobian_colSix()
{

    mat4 sh_rotz = getZRotationMatrix(theta_3_);
    mat4 sh_roty = getYRotationMatrix(theta_2_);
    mat4 sh_rotx = getXRotationMatrix(theta_1_);

    /*elbow rotation*/
    mat4 el_roty = getYRotationMatrix(theta_5_);
    mat4 el_rotx = getXRotationMatrix(theta_4_);
    /*wrist rotation*/
    // derivative on theta_6
    mat4 dt_wr_roty = getYRotationMatrixDev(theta_6_);
    mat4 wr_rotz = getZRotationMatrix(theta_7_);
    /*shoulder, elbow, and wrist translations*/
    mat4 sh_transl = getTranslationMatrix(1.0f, 1.0f, 1.0f);
    mat4 el_transl = getTranslationMatrix(1.0f, 1.0f, 1.0f);
    mat4 wr_transl = getTranslationMatrix(1.0f, 1.0f, 1.0f);
    mat4 rt_transl = mat4(1.0f);

    // the Jacobian matrix colum product to compute for.
    mat4 jMcolProduct(1.0f);


    // theta six Jacobian column multiplications
    jMcolProduct = rt_transl * sh_transl;
    jMcolProduct = jMcolProduct * sh_rotz;
    jMcolProduct = jMcolProduct * sh_roty;
    jMcolProduct = jMcolProduct * sh_rotx;
    jMcolProduct = jMcolProduct * el_transl;
    jMcolProduct = jMcolProduct * el_roty;
    jMcolProduct = jMcolProduct * el_rotx;
    jMcolProduct = jMcolProduct * wr_transl;
    jMcolProduct = jMcolProduct * wr_rotz;
    jMcolProduct = jMcolProduct * dt_wr_roty;

    vec3 dirct = hand_->getPosition();
    vec4 dir_t = vec4(dirct[0], dirct[1], dirct[2], 1);
    vec4 col_one = jMcolProduct * dir_t;
    return col_one;
}

/* Seventh Jacobian Column */
vec4 ArmSimulator::getJacobian_colSvn()
{

    mat4 sh_rotz = getZRotationMatrix(theta_3_);
    mat4 sh_roty = getYRotationMatrix(theta_2_);
    mat4 sh_rotx = getXRotationMatrix(theta_1_);

    /*elbow rotation*/
    mat4 el_roty = getYRotationMatrix(theta_5_);
    mat4 el_rotx = getXRotationMatrix(theta_4_);

    /*wrist rotation*/
    // derivative on theta_7
    mat4 wr_roty = getYRotationMatrix(theta_6_);
    mat4 dt_wr_rotz = getZRotationMatrixDev(theta_7_);
    /*shoulder, elbow, and wrist translations*/
    mat4 sh_transl = getTranslationMatrix(1.0f, 1.0f, 1.0f);
    mat4 el_transl = getTranslationMatrix(1.0f, 1.0f, 1.0f);
    mat4 wr_transl = getTranslationMatrix(1.0f, 1.0f, 1.0f);
    mat4 rt_transl = mat4(1.0f);

    // the Jacobian matrix colum product to compute for.
    mat4 jMcolProduct(1.0f);

    // theta seven Jacobian column multiplications
    jMcolProduct = rt_transl * sh_transl;
    jMcolProduct = jMcolProduct * sh_rotz;
    jMcolProduct = jMcolProduct * sh_roty;
    jMcolProduct = jMcolProduct * sh_rotx;
    jMcolProduct = jMcolProduct * el_transl;
    jMcolProduct = jMcolProduct * el_roty;
    jMcolProduct = jMcolProduct * el_rotx;
    jMcolProduct = jMcolProduct * wr_transl;
    jMcolProduct = jMcolProduct * dt_wr_rotz;
    jMcolProduct = jMcolProduct * wr_roty;

    vec3 dirct = hand_->getPosition();
    vec4 dir_t = vec4(dirct[0], dirct[1], dirct[2], 1);
    vec4 col_one = jMcolProduct * dir_t;
    return col_one;
}

/// <summary>
/// 
/// </summary>
/// <returns>
/// std::vector of type glm_vec4: (clm1_of Jacobian),...,(clm7 of Jacobian) </returns>
std::vector<vec4> ArmSimulator::computeJacobian()
{

    // jColOne: (dx/dThetaOne, dy/dThetaOne, dz/dThetaOne, 1)
    vec4 jColOne = getJacobian_colOne();
    vec4 jColTwo = getJacobian_colTwo();
    vec4 jColThree = getJacobian_colThree();
    vec4 jColFour = getJacobian_colFour();
    vec4 jColFive = getJacobian_colFive();
    vec4 jColSix = getJacobian_colSix();
    vec4 jColSvn = getJacobian_colSvn();

    std::vector<vec4> jCols;

    jCols.push_back(jColOne);
    jCols.push_back(jColTwo);
    jCols.push_back(jColThree);
    jCols.push_back(jColFour);
    jCols.push_back(jColFive);
    jCols.push_back(jColSix);
    jCols.push_back(jColSvn);

    return jCols;
}

/// <summary>
/// 
/// </summary>
/// <param name="mvec"> A glm vec3</param>
/// <returns> Returns an eigen 3f version of "mvec"</returns>
Eigen::Vector3f glVecToEigenVecThree(vec3 mvec)
{
    Eigen::Vector3f rt_eig_vec;
    rt_eig_vec << mvec[0], mvec[1], mvec[2];

    return rt_eig_vec;

}

/// <summary>
/// 
/// </summary>
/// <param name="mvec"> A glm vec3</param>
/// <returns> Returns an glm vec version of "mvec"</returns>
vec3 eignVecToGlVecThree(Eigen::Vector3f mvec)
{
    vec3 rt_gv = vec3(mvec[0], mvec[1], mvec[2]);
    return rt_gv;
}

/// <summary>
/// Rotate the first link
/// </summary>
/// <param name="position"></param>
/// <param name="theta_t"></param>
void ArmSimulator::rotateLink1(vec3 position, float theta_t) {
    float vec_it = 1.0;
    mat4 rotZ = getZRotationMatrix(theta_3_);
    mat4 rotY = getYRotationMatrix(theta_2_);
    mat4 rotX = getXRotationMatrix(theta_1_);
    mat4 rot = rotZ * rotY;
    rot = rot * rotX;
    vec3 lk1EndV3 = link1_->getEnd();
    vec4 lk1EndV4 = vec4(lk1EndV3[0], lk1EndV3[1], lk1EndV3[2], vec_it);
    vec3 start = link1_->getStart();
    vec4 rotatedDir = rot * lk1EndV4;
    float board_offset = 0.0;
    float temp_rot_dir = 0.0;
    if (rotatedDir[2] < 0)
    {
        temp_rot_dir = -1.0 * rotatedDir[2];
    }
    float board_z_offset = -0.2;
    board_offset = temp_rot_dir + board_z_offset + 0.2;
    vec3 rotatedDirV3 = vec3(rotatedDir[0], rotatedDir[1], (rotatedDir[2] - rotatedDir[2]));
    /*vec3 child_position = start + rotatedDirV3;*/
    vec3 child_position = normalize(rotatedDirV3);
    /*link1_->child->setPosition(child_position);
    link2_->parent->setPosition(child_position);*/
    link1_->child->setPosition(child_position);
    link2_->parent->setPosition(child_position);

}


/// <summary>
/// Rotate the second link
/// </summary>
/// <param name="position"></param>
/// <param name="theta_t"></param>
void ArmSimulator::rotateLink2(vec3 position, float theta_t) {
    float vec_it = 1.0;
    mat4 rotY = getYRotationMatrix(theta_5_);
    mat4 rotX = getXRotationMatrix(theta_4_);
    mat4 rot = rotY * rotX;
    vec3 lk2EndV3 = link2_->getEnd();
    vec4 lk2EndV4 = vec4(lk2EndV3[0], lk2EndV3[1], lk2EndV3[2], vec_it);
    vec3 start = link2_->getStart();
    vec4 rotatedDir = rot * lk2EndV4;
    /*vec3 child_position = start + rotatedDirV3;*/
    vec3 child_position = normalize(vec3(rotatedDir[0], rotatedDir[1], (rotatedDir[2] - rotatedDir[2])));

    /*link1_->child->setPosition(child_position);
    link2_->parent->setPosition(child_position);*/
    link2_->child->setPosition(child_position);
    link3_->parent->setPosition(child_position);
    /*link3_->parent->setPosition(child_position);*/
}

/// <summary>
/// Rotate the second link
/// </summary>
/// <param name="position"></param>
/// <param name="theta_t"></param>
void ArmSimulator::rotateLink3(vec3 position, float theta_t) {
    float vec_it = 1.0;
    mat4 rotY = getYRotationMatrix(theta_7_);
    mat4 rotZ = getZRotationMatrix(theta_6_);
    mat4 rot = rotY * rotZ;
    vec3 handPosV3 = hand_->getPosition();
    vec4 lk3EndV4 = vec4(handPosV3[0], handPosV3[1], handPosV3[2], vec_it);

    vec4 rotatedDir = rot * lk3EndV4;
    /*vec3 child_position = start + rotatedDirV3;*/
    vec3 child_position = normalize(vec3(rotatedDir[0], rotatedDir[1], (rotatedDir[2] - rotatedDir[2])));
    link3_->child->setPosition(child_position);
}


void ArmSimulator::mainCompute(int nextP)
{
    std::vector<vec4> jColumn;
    Eigen::Vector3f evalErr(1.0, 1.0, 1.0);
    Eigen::Vector3f threshold(0.05, 0.05, 0.05);
    float thresholdNorm = threshold.norm();
    // error is not less than real epsilon
    int newTrgtOK = 0;
    Eigen::MatrixXf jcbM(3, 7);

    Eigen::Vector3f targetP;

    /* Draw next point*/

    if (nextP < spl_points_.size())
    {
        targetP << spl_points_[nextP][0], spl_points_[nextP][1], spl_points_[nextP][2];
    }
    else
    {
        assert(False);
    }

    /*glm vectors have "_gv" at the end, otherwise it's an eigen vector*/
    vec3 hdPos_gv = hand_->getPosition();

    Eigen::Vector3f hdPos = glVecToEigenVecThree(hdPos_gv);
    evalErr = targetP - hdPos;
    newTrgtOK = 1;


    while (newTrgtOK)
    {
        Eigen::Vector3f kDx(evalErr[0], evalErr[1], evalErr[2]);
        kDx = epsK * kDx;
        /*get columns of the Jacobian type: glm::vec4 */
        jColumn = computeJacobian();


        /* create eigen jcbMatrix: jcbM*/
        for (int e = 0; e < 7; e++)
        {
            for (int i = 0; i < 3; i++)
            {
                /*jColumn(jColOne: dx/dt1->[0,0], dy/dt1->[0,1], dz/dt1->[0, 2], 1}, ...)*/
                /* col: 0 i: 0, col: 0 i: 1, col: 0 i: 2*/
                jcbM(i, e) = jColumn[e][i];
            }
        }
        Eigen::MatrixXf jcbMt(7, 3);
        jcbMt = jcbM.transpose();
        Eigen::Matrix3f jjT = jcbM * jcbMt;
        /*solve for Beta using (JJ^T)Beta = pDx*/
        Eigen::Vector3f jBeta = jjT.colPivHouseholderQr().solve(kDx);
        /*use (jjT)Beta to get dQ = dTheta*/
        Eigen::VectorXf dTheta(7);
        dTheta = jcbMt * jBeta;
        /*update Q: Q = Q + dQ*/
        theta_1_ = theta_1_ + dTheta[0];
        theta_2_ = theta_2_ + dTheta[1];
        theta_3_ = theta_3_ + dTheta[2];
        theta_4_ = theta_4_ + dTheta[3];
        theta_5_ = theta_5_ + dTheta[4];
        theta_6_ = theta_6_ + dTheta[5];
        theta_7_ = theta_7_ + dTheta[6];
        /**/
        vec3 kDx_gv = eignVecToGlVecThree(kDx);

        /* Update hand position by E.scale(k)*/
        hdPos_gv = hdPos_gv + kDx_gv;

        Eigen::Vector3f hdPosNew = glVecToEigenVecThree(hdPos_gv);
        evalErr = targetP - hdPosNew;
        /* length on new error */
        float lenErr = evalErr.norm();
        /* compare (len(evalErr) with len(espReal)*/
        if (lenErr < thresholdNorm)
        {
            /* End while loop go to the next spline sampled point*/
            newTrgtOK = 0;
            end_ = origin_ + hdPos_gv;
            // temp speed
            float tSpeed = 1.0;
            vec3 newHdPos_gv = hdPos_gv;

            control_point_->setPosition(newHdPos_gv);
            vec3 t_vec = vec3(0, 0, 0.5);       
            t_vec = newHdPos_gv + t_vec;
            hand_->setPosition(t_vec);

            vec3 relative_pos = newHdPos_gv - origin_;
            
            rotateLink1(relative_pos, tSpeed);
            rotateLink2(relative_pos, tSpeed);
            rotateLink3(relative_pos, tSpeed);
        }
    }

}



void ArmSimulator::loadSpline()
{
    hermiteSystem = new Hermite("hermite");
    const char* cFile = inFileName.c_str();
    hermiteSystem->loadFromFile2D(cFile);

    int splNPoints = hermiteSystem->size();

    for (int i = 0; i < splNPoints; i++)
    {
        double ti = 0.0;
        for (int j = 0; j < 40; j++)
        {
            VectorObj pointM = hermiteSystem->getIntermediatePoint(ti);
            vec3 a_point = vec3(pointM.x(), pointM.y(), 0.0);
            spl_points_.push_back(a_point);
            ti += 0.025;
        }

    }
    hasSplineOK = 1;
    int fLo = 1;
}

void ArmSimulator::displaySpline(float r, int sz)
{
    int isDone = 0;
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    glColor3f(1.0, 0.02, 0.8);
    glLineWidth(r);
    glBegin(GL_LINE_STRIP);
    /*the function loadSpline should have create 40 intermediate points*/
    int loopTo = spl_points_.size();
    for (int i = 0; i < loopTo; i++)
    {
        /*glVertex3f(samplePoints[i][0], samplePoints[i][1], samplePoints[i][2]);*/
        glVertex3f(spl_points_[i][0], spl_points_[i][1], 0.0);
    }
    isDone = 1;
    glEnd();

    glPopMatrix();
    glPopAttrib();
}


/// <summary>
/// Step in time
/// </summary>
/// <param name="time">the time step</param>
/// <returns></returns>
int ArmSimulator::step(double time) {
    /*float delta_t = time - prev_time;*/
    /*prev_time = 100 * time;*/
    hand_->setNumPoints(100);
    int allPoints = hand_->getNumPoints();

    /*For each sampled of the spline*/
    if (nextPoint_ < allPoints)
    {
        mainCompute(nextPoint_);
        /*vec3 ro_value = spl_points_[8];*/
        nextPoint_ = nextPoint_ + 1;
    }
    else
    {
        nextPoint_ = 0;
    }

    return 0;
}


int ArmSimulator::command(int argc, myCONST_SPEC char** argv)
{
    if (strcmp(argv[0], "read") == 0)
    {
        if (argc == 2) {
            inFileName = argv[1];
            loadSpline();
            animTcl::OutputMessage("File Ok!");
            glutPostRedisplay();
        }
        else {
            animTcl::OutputMessage("File not found");
            return TCL_OK;
        }

    }
    else
    {
        animTcl::OutputMessage("Unknown Simulator command.");
        return TCL_OK;
    }
    return TCL_OK;
}

/// <summary>
/// Display
/// </summary>
/// <param name="mode"></param>
void ArmSimulator::display(GLenum mode)
{
    control_point_->display(mode);
    hand_->display(mode);
    if (hasSplineOK == 1)
    {
        float r = 1.5;
        int numPoints = hermiteSystem->size();
        displaySpline(r, numPoints);
    }
}
