#pragma once
#define SECOND_FRAME 60 //1�b�̃t���[��
#define MINITS_FRAME SECOND_FRAME * 60//1���̃t���[��
static int MAX_LIFE_TIME_HAND_WRITING = 180;//�M�Ղ̐�������
static int MAX_HP_ENEMY_PENCIL = 10;//���M��HP
static int MAX_HP_ENEMY_MECHANICAL_PENCIL = 10;//�V���[�y����HP
static float WRITING_RADIUS = 1.5f;//�M�Ղ̋��̒��a(���a���̈ړ����Ƃɐ������邽�߁j
static int ENEMY_ATTACK_TIME = SECOND_FRAME;//�U���̎�������
static int ENEMY_BEFORE_ATTACK_TIME = SECOND_FRAME;//�U���O�̗��ߎ���
static int MAX_HP_BOSS = 75;//�{�X�̏���HP

const float maxShard = 100;//�˂肯���Q�[�W�̍ő�l
const int maxKneadedErasers = 40;//�˂肯���̗��̍ő吔
const float kneadedEraserDistance = 1.0f;//��������Ȃ�����Ƃ˂肯���̖��x���Z���Ȃ�A�ő�l�ɒB���₷���Ȃ�
const float maxMoveSpeed = 0.3;//�ō��ړ����x
const float maxRotateSpeed = 15;//��]���[�h�̍ő�p���x