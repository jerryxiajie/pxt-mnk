input.onButtonPressed(Button.B, () => {
    minode.FanControl_1(AnalogConnName.Analog_A2, 40)
})
input.onButtonPressed(Button.A, () => {
    minode.FanControl_1(AnalogConnName.Analog_A2, 80)
})
basic.forever(() => {
    basic.showNumber(minode.DHTGetTemperature(ConnName.D13, DHTTemStyle.MINODE_DHT_CELSIUS))
    basic.pause(100)
})