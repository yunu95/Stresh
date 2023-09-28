#pragma once
/// �ð��� �����ϴ� ���

// �ð��� ��Ÿ �ð��� �����Ͽ� Ÿ ��ü���� ������Ʈ �Լ����� ������ ���� �� �ֵ��� �մϴ�.
typedef struct
{
    double deltaTime;
    int FPS;
}
Time;

Time Time_Instance;

// �ð��� �ٷ�� ��� �ʱ�ȭ
void Time_Initialize();
// �ð� ������ ���̴� ������Ʈ �ֱ� �Լ��Դϴ�. ���� ���۵��� Time�� ���¿� �������̹Ƿ�, �� �Լ��� �ٸ� ��� ������Ʈ �Լ����� �� �켱������ ȣ��Ǿ�� �մϴ�.
void Time_Update();


// 2���� �ð� ������ �ð��� ��� �Լ�
/// Elasped Time 

// ���� �����Ӱ� ���� �����Ӱ��� �ɸ� �ð�
/// Elasped Time (DeltaTime)

// ������ ���� �Լ�
/// ����, 60FPS�� ��� deltaTime�� 16.7ms���� ������
/// �������� ��⸦ �Ѵ�.

void DebugTime();
