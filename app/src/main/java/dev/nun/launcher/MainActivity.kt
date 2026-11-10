package dev.nun.launcher

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import dev.nun.launcher.ui.LauncherHome
import dev.nun.launcher.ui.theme.NunTheme

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        window.statusBarColor = android.graphics.Color.rgb(13, 13, 13)
        window.navigationBarColor = android.graphics.Color.rgb(13, 13, 13)
        setContent {
            NunTheme {
                LauncherHome()
            }
        }
    }
}
