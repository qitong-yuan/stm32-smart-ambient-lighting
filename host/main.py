# -*- coding: utf-8 -*-
import sys
from PySide6.QtWidgets import QApplication, QMainWindow, QColorDialog
from PySide6.QtCore import Qt, QTimer
from PySide6.QtGui import QColor

from Ui_LEDController import Ui_LEDController

# 串口库
try:
    import serial
    import serial.tools.list_ports
except:
    serial = None

class LEDController(QMainWindow):
    def __init__(self):
        super().__init__()
        self.ui = Ui_LED_CONTROLLER = Ui_LEDController()
        self.ui.setupUi(self)

        # 串口对象
        self.serial_port = None

        # 默认颜色
        self. current_color = QColor(135, 206, 215)

        # 初始化控件初始值与信号
        self._init_widgets()
        self._bind_signals()

        # 定时读取串口数据
        self. read_timer = QTimer(self)
        self.read_timer.timeout.connect(self. read_serial_data)

        # 启动时刷新串口
        self.refresh_ports()
        # 禁用个性颜色组（默认智能模式）
        try:
            self.ui.groupCustomColor.setEnabled(False)
        except Exception:
            pass

    def _init_widgets(self):
        try:
            self.ui.sliderR.setRange(0, 255)
            self.ui.sliderG.setRange(0, 255)
            self.ui.sliderB.setRange(0, 255)

            self.ui.sliderR.setValue(135)
            self.ui.sliderG.setValue(206)
            self.ui.sliderB.setValue(215)

            self.ui.labelRValue.setText(str(self.ui.sliderR.value()))
            self.ui.labelGValue.setText(str(self.ui.sliderG.value()))
            self.ui.labelBValue.setText(str(self.ui.sliderB.value()))
        except Exception:
            pass

        self.update_color_preview()

    def _bind_signals(self):
        # 串口
        try:
            self.ui.btnRefresh.clicked.connect(self. refresh_ports)
            self. ui.btnConnect.clicked.connect(self.toggle_connection)
        except Exception:
            pass

        # 模式按钮
        try:
            self.ui.btnSmartMode.clicked.connect(self.switch_to_smart_mode)
            self.ui.btnCustomMode.clicked.connect(self.switch_to_custom_mode)
        except Exception:
            pass

        # 智能功能开关：每个切换都发送对应命令
        try:
            self.ui.checkTemp.stateChanged.connect(lambda _: self.send_command("TEMP", 1 if self.ui.checkTemp. isChecked() else 0))
            self.ui.checkHumi.stateChanged.connect(lambda _: self.send_command("HUMI", 1 if self.ui.checkHumi. isChecked() else 0))
            self.ui.checkWelcome.stateChanged.connect(lambda _: self.send_command("WELCOME", 1 if self.ui.checkWelcome. isChecked() else 0))
            self.ui.checkDrive.stateChanged.connect(lambda _: self.send_command("DRIVE", 1 if self.ui.checkDrive. isChecked() else 0))
        except Exception:
            pass

        # 滑块联动标签 & 预览
        try:
            self. ui.sliderR.valueChanged.connect(lambda v: self.ui.labelRValue.setText(str(v)))
            self.ui.sliderG.valueChanged.connect(lambda v: self.ui.labelGValue.setText(str(v)))
            self.ui.sliderB.valueChanged.connect(lambda v: self.ui.labelBValue.setText(str(v)))
            self.ui.sliderR. valueChanged.connect(self.update_color_preview)
            self.ui.sliderG.valueChanged.connect(self.update_color_preview)
            self.ui.sliderB.valueChanged.connect(self.update_color_preview)
        except Exception:
            pass

        # 颜色选择与应用
        try:
            self.ui.btnPickColor.clicked.connect(self. pick_color)
            self.ui.btnApplyColor.clicked.connect(self.apply_color)
        except Exception:
            pass

        # 预设场景单选按钮
        try:
            self.ui.normal.clicked.connect(lambda: self.select_scene(1))
            self.ui.long_trip_drive.clicked.connect(lambda: self.select_scene(2))
            self.ui.romantic_scene.clicked.connect(lambda: self.select_scene(3))
            self.ui.emergency_scene.clicked.connect(lambda: self.select_scene(4))
        except Exception as e:
            print(f"预设场景绑定失败: {e}")

        # 灯光单选按钮
        try:
            self.ui.steady_effect.clicked.connect(lambda: self.select_light(1))
            self.ui.breathing_effect.clicked.connect(lambda: self.select_light(2))
            self.ui.blinking_effect.clicked.connect(lambda: self.select_light(3))
            self.ui.flowing_effect.clicked.connect(lambda: self.select_light(4))
        except Exception as e:
            print(f"灯光绑定失败: {e}")

    def select_scene(self, scene_id):
        """选择预设场景"""
        self.send_command("SCENE", scene_id)
        scenes = {1: "默认", 2: "长途驾驶", 3: "氛围", 4: "紧急"}
        self.ui.labelReceive.setText(f"→ 切换场景: {scenes. get(scene_id, '未知')}")

    def select_light(self, light_id):
        """选择预设场景"""
        self.send_command("LIGHT", light_id)
        lights = {1: "默认", 2: "呼吸", 3: "闪烁", 4: "流水"}
        self.ui.labelReceive.setText(f"→ 切换场景: {lights. get(light_id, '未知')}")

    def update_color_preview(self):
        """根据滑块更新预览标签背景色"""
        try:
            r = self.ui.sliderR.value()
            g = self.ui.sliderG.value()
            b = self.ui.sliderB.value()
            self.current_color = QColor(r, g, b)
            self.ui.labelPreview.setStyleSheet(f"background-color: rgb({r},{g},{b}); border: 1px solid #000;")
        except Exception:
            pass

    def pick_color(self):
        color = QColorDialog.getColor(self.current_color, self, "选择颜色")
        if color.isValid():
            try:
                self.ui.sliderR.setValue(color.red())
                self.ui.sliderG.setValue(color.green())
                self.ui.sliderB.setValue(color.blue())
            except Exception:
                self.current_color = color
                try:
                    self.ui. labelPreview.setStyleSheet(f"background-color: {color.name()};")
                except Exception:
                    pass

    def refresh_ports(self):
        try:
            self.ui.comboPort.clear()
            if serial is None:
                return
            ports = serial.tools.list_ports.comports()
            for port in ports:
                self.ui.comboPort.addItem(f"{port.device} - {port.description}")
            if not ports:
                self.ui.comboPort.addItem("未发现可用串口")
        except Exception as e:
            self.ui.comboPort.clear()

    def toggle_connection(self):
        if serial is None:
            self.ui.labelStatus.setText("pyserial 未安装")
            return

        if self.serial_port and self.serial_port.is_open:
            try:
                self.serial_port.close()
            except Exception:
                pass
            self.serial_port = None
            self.ui.btnConnect.setText("连接")
            self. ui.labelStatus.setText("未连接")
            self.ui.labelStatus.setStyleSheet("color: red;")
            self.read_timer.stop()
            return

        text = self.ui.comboPort.currentText()
        if not text or "未发现" in text or "未安装" in text or "错误" in text:
            self.ui.labelStatus.setText("请选择有效端口")
            self.ui. labelStatus.setStyleSheet("color: red;")
            return

        port_name = text.split(" - ")[0]
        try:
            self.serial_port = serial.Serial(port_name, 115200, timeout=0.1)
            self.ui.btnConnect.setText("断开")
            self.ui.labelStatus. setText("已连接")
            self.ui.labelStatus.setStyleSheet("color: green;")
            # 启动读线程（定时）
            self.read_timer. start(100)
        except Exception as e:
            self.ui.labelStatus.setText(f"连接失败: {e}")
            self.ui. labelStatus.setStyleSheet("color: red;")
            self.serial_port = None

    def send_command(self, cmd, *params):
        """封装：#CMD,param1,param2*"""
        if self.serial_port and self.serial_port.is_open:
            command = f"#{cmd}"
            for p in params:
                command += f",{p}"
            command += "*"
            try:
                self.serial_port.write(command.encode())
                print("发送:", command)
            except Exception as e:
                print("发送失败:", e)
        else:
            command = f"#{cmd}" + "". join(f",{p}" for p in params) + "*"
            print("未连接，拟发送:", command)

    def switch_to_smart_mode(self):
        """切换到智能模式"""
        try:
            self.ui.btnCustomMode.setChecked(False)
            self.ui.groupSmart.setEnabled(True)
            self.ui.smart_scene_group.setEnabled(True)
            self.ui.groupCustomColor.setEnabled(False)
            self.ui.groupLight.setEnabled(False)
            self.send_command("MODE", 1)
            # 文本显示标识
            self.ui.btnSmartMode.setText("智能模式 ✓")
            self.ui. btnCustomMode.setText("个性模式")
        except Exception:
            pass

    def switch_to_custom_mode(self):
        """切换到个性模式"""
        try:
            self.ui. btnSmartMode.setChecked(False)
            self.ui.groupSmart.setEnabled(False)
            self.ui.smart_scene_group.setEnabled(False)
            self.ui.groupCustomColor.setEnabled(True)
            self.ui.groupLight.setEnabled(True)
            self.send_command("MODE", 0)
            self.ui.btnSmartMode.setText("智能模式")
            self.ui.btnCustomMode.setText("个性模式 ✓")
        except Exception:
            pass

    def apply_color(self):
        """应用当前颜色"""
        try:
            r = self.ui.sliderR.value()
            g = self.ui.sliderG. value()
            b = self. ui.sliderB.value()
            self.send_command("COLOR", r, g, b)
        except Exception:
            pass

    def read_serial_data(self):
        """周期性从串口读取并显示"""
        if not (self.serial_port and self. serial_port.is_open):
            return
        try:
            if self.serial_port.in_waiting > 0:
                data = self.serial_port. readline().decode('utf-8', errors='ignore').strip()
                if data:
                    self. ui.labelReceive.setText(f"收到: {data}")
        except Exception as e:
            # 出错不崩溃，仅打印日志
            print("读取串口数据错误:", e)


if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = LEDController()
    window.show()
    sys.exit(app.exec())