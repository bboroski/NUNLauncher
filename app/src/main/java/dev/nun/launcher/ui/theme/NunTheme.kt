package dev.nun.launcher.ui.theme

import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.darkColorScheme
import androidx.compose.runtime.Composable
import androidx.compose.ui.graphics.Color

val NunGraphite = Color(0xFF0D0D0D)
val NunPanel = Color(0xFF1A1A1A)
val NunYellow = Color(0xFFFFD600)
val NunYellowMuted = Color(0xFFBFA100)
val NunText = Color(0xFFE6E6E6)
val NunSecondaryText = Color(0xFF999999)
val NunDivider = Color(0xFF262626)
val NunError = Color(0xFFFF4444)
val NunSuccess = Color(0xFF4CAF50)

private val NunColors = darkColorScheme(
    primary = NunYellow,
    onPrimary = NunGraphite,
    secondary = NunYellowMuted,
    onSecondary = NunGraphite,
    background = NunGraphite,
    onBackground = NunText,
    surface = NunPanel,
    onSurface = NunText,
    surfaceVariant = Color(0xFF222222),
    onSurfaceVariant = NunSecondaryText,
    outline = NunDivider,
    error = NunError,
    onError = NunGraphite
)

@Composable
fun NunTheme(content: @Composable () -> Unit) {
    MaterialTheme(
        colorScheme = NunColors,
        content = content
    )
}
