package com.example.catchmeifyoucan.views

import android.content.Context
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import android.util.AttributeSet
import android.view.View

class RandomYellowHeadlightBeamView : View {
    constructor(context: Context?, attrs: AttributeSet?) : super(context, attrs)

    private val paint = Paint()

    override fun onDraw(canvas: Canvas?) {
        paint.isAntiAlias = true
        paint.color = Color.YELLOW
        canvas?.drawCircle(30f, 30f, 30f, paint)
    }
}