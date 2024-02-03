def calculate_mecanum_wheel_speeds(Vx, Vy, omega, L, R):
    """
    计算麦轮驱动的机器人每个轮子的线速度。

    参数:
    - Vx: 机器人在x方向上的速度。
    - Vy: 机器人在y方向上的速度。
    - omega: 机器人绕z轴的旋转速率。
    - L: 机器人中心到轮子中心的距离。
    - R: 轮子的半径。

    返回:
    - 一个包含四个轮子线速度的元组。
    """
    V1 = (Vx - Vy - omega * L) * R
    V2 = (Vx + Vy + omega * L) * R
    V3 = (Vx + Vy - omega * L) * R
    V4 = (Vx - Vy + omega * L) * R

    return V1, V2, V3, V4

# 示例参数
Vx = 1.0  # x方向速度
Vy = 1.0  # y方向速度
omega = 0  # 旋转速率
L = 0.1768  # 中心到轮子的距离
R = 0.05  # 轮子半径

# 计算并打印每个轮子的线速度
wheel_speeds = calculate_mecanum_wheel_speeds(Vx, Vy, omega, L, R)
print(f"Wheel Speeds: {wheel_speeds}")
