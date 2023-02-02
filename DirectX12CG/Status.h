#pragma once
#define SECOND_FRAME 60 //1秒のフレーム
#define MINITS_FRAME SECOND_FRAME * 60//1分のフレーム
static int MAX_LIFE_TIME_HAND_WRITING = 180;//筆跡の生存時間
static int MAX_HP_ENEMY_PENCIL = 10;//鉛筆のHP
static int MAX_HP_ENEMY_MECHANICAL_PENCIL = 10;//シャーペンのHP
static float WRITING_RADIUS = 1.5f;//筆跡の球の直径(直径分の移動ごとに生成するため）
static int ENEMY_ATTACK_TIME = SECOND_FRAME;//攻撃の持続時間
static int ENEMY_BEFORE_ATTACK_TIME = SECOND_FRAME;//攻撃前の溜め時間
static int MAX_HP_BOSS = 75;//ボスの初期HP

const float maxShard = 100;//ねりけしゲージの最大値
const int maxKneadedErasers = 40;//ねりけしの粒の最大数
const float kneadedEraserDistance = 1.0f;//これを少なくするとねりけしの密度が濃くなり、最大値に達しやすくなる
const float maxMoveSpeed = 0.3;//最高移動速度
const float maxRotateSpeed = 15;//回転モードの最大角速度