# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'LEDController.ui'
##
## Created by: Qt User Interface Compiler version 6.10.0
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

from PySide6.QtCore import (QCoreApplication, QDate, QDateTime, QLocale,
    QMetaObject, QObject, QPoint, QRect,
    QSize, QTime, QUrl, Qt)
from PySide6.QtGui import (QBrush, QColor, QConicalGradient, QCursor,
    QFont, QFontDatabase, QGradient, QIcon,
    QImage, QKeySequence, QLinearGradient, QPainter,
    QPalette, QPixmap, QRadialGradient, QTransform)
from PySide6.QtWidgets import (QApplication, QCheckBox, QComboBox, QFormLayout,
    QGroupBox, QHBoxLayout, QLabel, QMainWindow,
    QPushButton, QRadioButton, QSizePolicy, QSlider,
    QVBoxLayout, QWidget)

class Ui_LEDController(object):
    def setupUi(self, LEDController):
        if not LEDController.objectName():
            LEDController.setObjectName(u"LEDController")
        LEDController.resize(493, 747)
        self.centralwidget = QWidget(LEDController)
        self.centralwidget.setObjectName(u"centralwidget")
        self.verticalLayout_main = QVBoxLayout(self.centralwidget)
        self.verticalLayout_main.setObjectName(u"verticalLayout_main")
        self.groupSerial = QGroupBox(self.centralwidget)
        self.groupSerial.setObjectName(u"groupSerial")
        self.layoutSerial = QHBoxLayout(self.groupSerial)
        self.layoutSerial.setObjectName(u"layoutSerial")
        self.labelPort = QLabel(self.groupSerial)
        self.labelPort.setObjectName(u"labelPort")

        self.layoutSerial.addWidget(self.labelPort)

        self.comboPort = QComboBox(self.groupSerial)
        self.comboPort.setObjectName(u"comboPort")

        self.layoutSerial.addWidget(self.comboPort)

        self.btnRefresh = QPushButton(self.groupSerial)
        self.btnRefresh.setObjectName(u"btnRefresh")

        self.layoutSerial.addWidget(self.btnRefresh)

        self.btnConnect = QPushButton(self.groupSerial)
        self.btnConnect.setObjectName(u"btnConnect")

        self.layoutSerial.addWidget(self.btnConnect)

        self.labelStatus = QLabel(self.groupSerial)
        self.labelStatus.setObjectName(u"labelStatus")

        self.layoutSerial.addWidget(self.labelStatus)


        self.verticalLayout_main.addWidget(self.groupSerial)

        self.groupMode = QGroupBox(self.centralwidget)
        self.groupMode.setObjectName(u"groupMode")
        self.layoutMode = QVBoxLayout(self.groupMode)
        self.layoutMode.setObjectName(u"layoutMode")
        self.btnSmartMode = QPushButton(self.groupMode)
        self.btnSmartMode.setObjectName(u"btnSmartMode")
        self.btnSmartMode.setCheckable(True)

        self.layoutMode.addWidget(self.btnSmartMode)

        self.btnCustomMode = QPushButton(self.groupMode)
        self.btnCustomMode.setObjectName(u"btnCustomMode")
        self.btnCustomMode.setCheckable(True)

        self.layoutMode.addWidget(self.btnCustomMode)


        self.verticalLayout_main.addWidget(self.groupMode)

        self.groupBox = QGroupBox(self.centralwidget)
        self.groupBox.setObjectName(u"groupBox")
        self.formLayout_3 = QFormLayout(self.groupBox)
        self.formLayout_3.setObjectName(u"formLayout_3")
        self.groupSmart = QGroupBox(self.groupBox)
        self.groupSmart.setObjectName(u"groupSmart")
        self.formLayout_2 = QFormLayout(self.groupSmart)
        self.formLayout_2.setObjectName(u"formLayout_2")
        self.checkTemp = QCheckBox(self.groupSmart)
        self.checkTemp.setObjectName(u"checkTemp")

        self.formLayout_2.setWidget(0, QFormLayout.ItemRole.LabelRole, self.checkTemp)

        self.checkHumi = QCheckBox(self.groupSmart)
        self.checkHumi.setObjectName(u"checkHumi")

        self.formLayout_2.setWidget(0, QFormLayout.ItemRole.FieldRole, self.checkHumi)

        self.checkWelcome = QCheckBox(self.groupSmart)
        self.checkWelcome.setObjectName(u"checkWelcome")

        self.formLayout_2.setWidget(1, QFormLayout.ItemRole.LabelRole, self.checkWelcome)

        self.checkDrive = QCheckBox(self.groupSmart)
        self.checkDrive.setObjectName(u"checkDrive")

        self.formLayout_2.setWidget(1, QFormLayout.ItemRole.FieldRole, self.checkDrive)


        self.formLayout_3.setWidget(0, QFormLayout.ItemRole.LabelRole, self.groupSmart)

        self.smart_scene_group = QGroupBox(self.groupBox)
        self.smart_scene_group.setObjectName(u"smart_scene_group")
        self.formLayout = QFormLayout(self.smart_scene_group)
        self.formLayout.setObjectName(u"formLayout")
        self.normal = QRadioButton(self.smart_scene_group)
        self.normal.setObjectName(u"normal")

        self.formLayout.setWidget(0, QFormLayout.ItemRole.LabelRole, self.normal)

        self.long_trip_drive = QRadioButton(self.smart_scene_group)
        self.long_trip_drive.setObjectName(u"long_trip_drive")

        self.formLayout.setWidget(1, QFormLayout.ItemRole.LabelRole, self.long_trip_drive)

        self.romantic_scene = QRadioButton(self.smart_scene_group)
        self.romantic_scene.setObjectName(u"romantic_scene")

        self.formLayout.setWidget(0, QFormLayout.ItemRole.FieldRole, self.romantic_scene)

        self.emergency_scene = QRadioButton(self.smart_scene_group)
        self.emergency_scene.setObjectName(u"emergency_scene")

        self.formLayout.setWidget(1, QFormLayout.ItemRole.FieldRole, self.emergency_scene)


        self.formLayout_3.setWidget(0, QFormLayout.ItemRole.FieldRole, self.smart_scene_group)


        self.verticalLayout_main.addWidget(self.groupBox)

        self.groupBox_3 = QGroupBox(self.centralwidget)
        self.groupBox_3.setObjectName(u"groupBox_3")
        self.horizontalLayout = QHBoxLayout(self.groupBox_3)
        self.horizontalLayout.setObjectName(u"horizontalLayout")
        self.groupCustomColor = QGroupBox(self.groupBox_3)
        self.groupCustomColor.setObjectName(u"groupCustomColor")
        sizePolicy = QSizePolicy(QSizePolicy.Policy.Preferred, QSizePolicy.Policy.Preferred)
        sizePolicy.setHorizontalStretch(4)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.groupCustomColor.sizePolicy().hasHeightForWidth())
        self.groupCustomColor.setSizePolicy(sizePolicy)
        self.layoutCustom = QVBoxLayout(self.groupCustomColor)
        self.layoutCustom.setObjectName(u"layoutCustom")
        self.layoutR = QHBoxLayout()
        self.layoutR.setObjectName(u"layoutR")
        self.labelR = QLabel(self.groupCustomColor)
        self.labelR.setObjectName(u"labelR")

        self.layoutR.addWidget(self.labelR)

        self.sliderR = QSlider(self.groupCustomColor)
        self.sliderR.setObjectName(u"sliderR")
        self.sliderR.setOrientation(Qt.Orientation.Horizontal)

        self.layoutR.addWidget(self.sliderR)

        self.labelRValue = QLabel(self.groupCustomColor)
        self.labelRValue.setObjectName(u"labelRValue")

        self.layoutR.addWidget(self.labelRValue)


        self.layoutCustom.addLayout(self.layoutR)

        self.layoutG = QHBoxLayout()
        self.layoutG.setObjectName(u"layoutG")
        self.labelG = QLabel(self.groupCustomColor)
        self.labelG.setObjectName(u"labelG")

        self.layoutG.addWidget(self.labelG)

        self.sliderG = QSlider(self.groupCustomColor)
        self.sliderG.setObjectName(u"sliderG")
        self.sliderG.setOrientation(Qt.Orientation.Horizontal)

        self.layoutG.addWidget(self.sliderG)

        self.labelGValue = QLabel(self.groupCustomColor)
        self.labelGValue.setObjectName(u"labelGValue")

        self.layoutG.addWidget(self.labelGValue)


        self.layoutCustom.addLayout(self.layoutG)

        self.layoutB = QHBoxLayout()
        self.layoutB.setObjectName(u"layoutB")
        self.labelB = QLabel(self.groupCustomColor)
        self.labelB.setObjectName(u"labelB")

        self.layoutB.addWidget(self.labelB)

        self.sliderB = QSlider(self.groupCustomColor)
        self.sliderB.setObjectName(u"sliderB")
        self.sliderB.setOrientation(Qt.Orientation.Horizontal)

        self.layoutB.addWidget(self.sliderB)

        self.labelBValue = QLabel(self.groupCustomColor)
        self.labelBValue.setObjectName(u"labelBValue")

        self.layoutB.addWidget(self.labelBValue)


        self.layoutCustom.addLayout(self.layoutB)

        self.layoutPreview = QHBoxLayout()
        self.layoutPreview.setObjectName(u"layoutPreview")
        self.labelPreviewText = QLabel(self.groupCustomColor)
        self.labelPreviewText.setObjectName(u"labelPreviewText")

        self.layoutPreview.addWidget(self.labelPreviewText)

        self.labelPreview = QLabel(self.groupCustomColor)
        self.labelPreview.setObjectName(u"labelPreview")
        self.labelPreview.setMinimumSize(QSize(100, 50))
        self.labelPreview.setStyleSheet(u"background-color: rgb(135,206,215);")

        self.layoutPreview.addWidget(self.labelPreview)

        self.btnPickColor = QPushButton(self.groupCustomColor)
        self.btnPickColor.setObjectName(u"btnPickColor")

        self.layoutPreview.addWidget(self.btnPickColor)


        self.layoutCustom.addLayout(self.layoutPreview)

        self.btnApplyColor = QPushButton(self.groupCustomColor)
        self.btnApplyColor.setObjectName(u"btnApplyColor")

        self.layoutCustom.addWidget(self.btnApplyColor)


        self.horizontalLayout.addWidget(self.groupCustomColor)

        self.groupLight = QGroupBox(self.groupBox_3)
        self.groupLight.setObjectName(u"groupLight")
        sizePolicy1 = QSizePolicy(QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Preferred)
        sizePolicy1.setHorizontalStretch(0)
        sizePolicy1.setVerticalStretch(0)
        sizePolicy1.setHeightForWidth(self.groupLight.sizePolicy().hasHeightForWidth())
        self.groupLight.setSizePolicy(sizePolicy1)
        self.groupLight.setMaximumSize(QSize(80000, 16777215))
        self.verticalLayout = QVBoxLayout(self.groupLight)
        self.verticalLayout.setObjectName(u"verticalLayout")
        self.steady_effect = QRadioButton(self.groupLight)
        self.steady_effect.setObjectName(u"steady_effect")

        self.verticalLayout.addWidget(self.steady_effect)

        self.breathing_effect = QRadioButton(self.groupLight)
        self.breathing_effect.setObjectName(u"breathing_effect")

        self.verticalLayout.addWidget(self.breathing_effect)

        self.blinking_effect = QRadioButton(self.groupLight)
        self.blinking_effect.setObjectName(u"blinking_effect")

        self.verticalLayout.addWidget(self.blinking_effect)

        self.flowing_effect = QRadioButton(self.groupLight)
        self.flowing_effect.setObjectName(u"flowing_effect")

        self.verticalLayout.addWidget(self.flowing_effect)


        self.horizontalLayout.addWidget(self.groupLight)


        self.verticalLayout_main.addWidget(self.groupBox_3)

        self.groupReceive = QGroupBox(self.centralwidget)
        self.groupReceive.setObjectName(u"groupReceive")
        self.layoutReceive = QVBoxLayout(self.groupReceive)
        self.layoutReceive.setObjectName(u"layoutReceive")
        self.labelReceive = QLabel(self.groupReceive)
        self.labelReceive.setObjectName(u"labelReceive")

        self.layoutReceive.addWidget(self.labelReceive)


        self.verticalLayout_main.addWidget(self.groupReceive)

        LEDController.setCentralWidget(self.centralwidget)

        self.retranslateUi(LEDController)

        QMetaObject.connectSlotsByName(LEDController)
    # setupUi

    def retranslateUi(self, LEDController):
        LEDController.setWindowTitle(QCoreApplication.translate("LEDController", u"\u8f66\u8f7d\u667a\u80fd\u7167\u660e\u7cfb\u7edf", None))
        self.groupSerial.setTitle(QCoreApplication.translate("LEDController", u"\u4e32\u53e3\u8fde\u63a5", None))
        self.labelPort.setText(QCoreApplication.translate("LEDController", u"\u7aef\u53e3:", None))
        self.btnRefresh.setText(QCoreApplication.translate("LEDController", u"\u5237\u65b0", None))
        self.btnConnect.setText(QCoreApplication.translate("LEDController", u"\u8fde\u63a5", None))
        self.labelStatus.setText(QCoreApplication.translate("LEDController", u"\u672a\u8fde\u63a5", None))
        self.groupMode.setTitle(QCoreApplication.translate("LEDController", u"\u5de5\u4f5c\u6a21\u5f0f", None))
        self.btnSmartMode.setText(QCoreApplication.translate("LEDController", u"\u667a\u80fd\u6a21\u5f0f ", None))
        self.btnCustomMode.setText(QCoreApplication.translate("LEDController", u"\u4e2a\u6027\u6a21\u5f0f", None))
        self.groupBox.setTitle(QCoreApplication.translate("LEDController", u"\u667a\u80fd\u6a21\u5f0f\u8bbe\u7f6e", None))
        self.groupSmart.setTitle(QCoreApplication.translate("LEDController", u"\u529f\u80fd", None))
        self.checkTemp.setText(QCoreApplication.translate("LEDController", u"\u6e29\u5ea6\u9002\u5e94", None))
        self.checkHumi.setText(QCoreApplication.translate("LEDController", u"\u6e7f\u5ea6\u9002\u5e94", None))
        self.checkWelcome.setText(QCoreApplication.translate("LEDController", u"\u8fce\u5bbe\u6a21\u5f0f", None))
        self.checkDrive.setText(QCoreApplication.translate("LEDController", u"\u9a7e\u9a76\u6a21\u5f0f", None))
        self.smart_scene_group.setTitle(QCoreApplication.translate("LEDController", u"\u9884\u8bbe\u573a\u666f", None))
        self.normal.setText(QCoreApplication.translate("LEDController", u"\u9ed8\u8ba4\u6a21\u5f0f", None))
        self.long_trip_drive.setText(QCoreApplication.translate("LEDController", u"\u957f\u9014\u6a21\u5f0f", None))
        self.romantic_scene.setText(QCoreApplication.translate("LEDController", u"\u6c1b\u56f4\u6a21\u5f0f", None))
        self.emergency_scene.setText(QCoreApplication.translate("LEDController", u"\u7d27\u6025\u6a21\u5f0f", None))
        self.groupBox_3.setTitle(QCoreApplication.translate("LEDController", u"\u4e2a\u6027\u6a21\u5f0f\u8bbe\u7f6e", None))
        self.groupCustomColor.setTitle(QCoreApplication.translate("LEDController", u"\u4e2a\u6027\u989c\u8272\u8bbe\u7f6e", None))
        self.labelR.setText(QCoreApplication.translate("LEDController", u"\u7ea2\u8272 (R):", None))
        self.labelRValue.setText(QCoreApplication.translate("LEDController", u"135", None))
        self.labelG.setText(QCoreApplication.translate("LEDController", u"\u7eff\u8272 (G):", None))
        self.labelGValue.setText(QCoreApplication.translate("LEDController", u"206", None))
        self.labelB.setText(QCoreApplication.translate("LEDController", u"\u84dd\u8272 (B):", None))
        self.labelBValue.setText(QCoreApplication.translate("LEDController", u"215", None))
        self.labelPreviewText.setText(QCoreApplication.translate("LEDController", u"\u989c\u8272\u9884\u89c8:", None))
        self.btnPickColor.setText(QCoreApplication.translate("LEDController", u"\u9009\u62e9\u989c\u8272", None))
        self.btnApplyColor.setText(QCoreApplication.translate("LEDController", u"\u5e94\u7528\u989c\u8272", None))
        self.groupLight.setTitle(QCoreApplication.translate("LEDController", u"\u706f\u5149", None))
        self.steady_effect.setText(QCoreApplication.translate("LEDController", u"\u9ed8\u8ba4\u6a21\u5f0f", None))
        self.breathing_effect.setText(QCoreApplication.translate("LEDController", u"\u547c\u5438\u6a21\u5f0f", None))
        self.blinking_effect.setText(QCoreApplication.translate("LEDController", u"\u95ea\u70c1\u6a21\u5f0f", None))
        self.flowing_effect.setText(QCoreApplication.translate("LEDController", u"\u6d41\u6c34\u6a21\u5f0f", None))
        self.groupReceive.setTitle(QCoreApplication.translate("LEDController", u"\u63a5\u6536\u6570\u636e", None))
        self.labelReceive.setText(QCoreApplication.translate("LEDController", u"\u7b49\u5f85\u6570\u636e...", None))
    # retranslateUi

