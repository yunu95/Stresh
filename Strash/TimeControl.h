#pragma once
/// �ð��� �����ϴ� ���

double TimeControl_deltaTime;
int TimeControl_FPS;
// �ð��� �ٷ�� ��� �ʱ�ȭ
void TimeControl_Initialize();
void TimeControl_Update();


// 2���� �ð� ������ �ð��� ��� �Լ�
/// Elasped Time 

// ���� �����Ӱ� ���� �����Ӱ��� �ɸ� �ð�
/// Elasped Time (DeltaTime)

// ������ ���� �Լ�
/// ����, 60FPS�� ��� deltaTime�� 16.7ms���� ������
/// �������� ��⸦ �Ѵ�.


