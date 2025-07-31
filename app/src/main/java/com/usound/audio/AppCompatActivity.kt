class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        NativeBridge.nativeStartEngine(48000, 2, 192)

        // Ejemplos
        freqSlider.addOnChangeListener { _, v, _ -> NativeBridge.nativeSetSineFreq(v) }
        gainSlider.addOnChangeListener { _, v, _ -> NativeBridge.nativeSetSineGain(v) }

        // Enviar el tono solo al oído izquierdo
        leftButton.setOnClickListener { NativeBridge.setSineEar(Ear.LEFT) }
        rightButton.setOnClickListener { NativeBridge.setSineEar(Ear.RIGHT) }
        bothButton.setOnClickListener { NativeBridge.setSineEar(Ear.BOTH) }

        // Ruido solo a la derecha
        noiseRightBtn.setOnClickListener { NativeBridge.setNoiseEar(Ear.RIGHT, 0.5f) }
    }

    override fun onDestroy() {
        NativeBridge.nativeStopEngine()
        super.onDestroy()
    }
}