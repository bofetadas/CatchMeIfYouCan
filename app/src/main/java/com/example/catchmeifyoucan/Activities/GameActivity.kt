package com.example.catchmeifyoucan.Activities

import android.content.Intent
import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import android.view.View
import android.view.Window
import com.example.catchmeifyoucan.Game.GameController
import com.example.catchmeifyoucan.Game.gameDuration
import com.example.catchmeifyoucan.R
import kotlinx.android.synthetic.main.game.*
import kotlinx.coroutines.GlobalScope
import kotlinx.coroutines.delay
import kotlinx.coroutines.launch

class GameActivity : AppCompatActivity(){
    private val gameController = GameController()
    private lateinit var views : Array<View>

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        requestWindowFeature(Window.FEATURE_NO_TITLE)
        setContentView(R.layout.game)
        window.decorView.apply {
            systemUiVisibility =
                View.SYSTEM_UI_FLAG_HIDE_NAVIGATION or View.SYSTEM_UI_FLAG_FULLSCREEN or View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
        }

        views = arrayOf(
            joystickView,
            playerHeadlightBeamView,
            randomGreenHeadlightBeamView,
            randomYellowHeadlightBeamView,
            randomRedHeadlightBeamView,
            score,
            time
        )

        val context = applicationContext
        gameController.setContext(context);
        startGame()
    }

    fun getGameController(): GameController {
        return gameController
    }

    private fun startGame() {
        gameController.initializeGameController(views)
        GlobalScope.launch {
            delay(gameDuration)
            gameController.endGame()
            goBackToMenu()
        }
    }
    private fun goBackToMenu(){
        val startScoreActivity = Intent(this, ScoreActivity::class.java)
        startScoreActivity.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP)
        startScoreActivity.addFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP)
        startScoreActivity.putExtra("score", gameController.getScore().toString())
        startActivity(startScoreActivity)
        finish()
    }
}