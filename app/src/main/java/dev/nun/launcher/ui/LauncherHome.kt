package dev.nun.launcher.ui

import android.content.ActivityNotFoundException
import android.content.Context
import android.content.pm.LauncherActivityInfo
import android.content.pm.LauncherApps
import android.os.Process
import android.widget.Toast
import android.content.Intent
import android.provider.Settings
import android.graphics.Bitmap
import android.graphics.Canvas
import android.graphics.drawable.Drawable
import androidx.compose.foundation.Image
import androidx.compose.foundation.background
import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.asPaddingValues
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.PaddingValues
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.WindowInsets
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.layout.width
import androidx.compose.foundation.layout.safeDrawing
import androidx.compose.foundation.lazy.grid.GridCells
import androidx.compose.foundation.lazy.grid.LazyVerticalGrid
import androidx.compose.foundation.lazy.grid.items
import androidx.compose.foundation.shape.CircleShape
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.foundation.text.BasicTextField
import androidx.compose.foundation.text.KeyboardOptions
import androidx.compose.foundation.text.KeyboardActions
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Refresh
import androidx.compose.material.icons.filled.Search
import androidx.compose.material.icons.filled.Settings
import androidx.compose.material3.Card
import androidx.compose.material3.CardDefaults
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButton
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.rememberCoroutineScope
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.asImageBitmap
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.graphics.SolidColor
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.text.input.ImeAction
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import androidx.compose.ui.platform.LocalContext
import dev.nun.launcher.R
import dev.nun.launcher.ui.theme.NunGraphite
import dev.nun.launcher.ui.theme.NunPanel
import dev.nun.launcher.ui.theme.NunSecondaryText
import dev.nun.launcher.ui.theme.NunText
import dev.nun.launcher.ui.theme.NunYellow
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext

private data class LaunchableApp(
    val label: String,
    val activity: LauncherActivityInfo,
    val icon: Drawable
)

@Composable
fun LauncherHome() {
    val context = LocalContext.current
    val scope = rememberCoroutineScope()
    var apps by remember { mutableStateOf(emptyList<LaunchableApp>()) }
    var query by remember { mutableStateOf("") }
    var loading by remember { mutableStateOf(true) }

    suspend fun reloadApps() {
        loading = true
        apps = withContext(Dispatchers.IO) { queryLaunchableApps(context) }
        loading = false
    }

    LaunchedEffect(Unit) { reloadApps() }

    val filtered = remember(apps, query) {
        if (query.isBlank()) apps
        else apps.filter { it.label.contains(query.trim(), ignoreCase = true) }
    }

    Column(
        modifier = Modifier
            .fillMaxSize()
            .background(NunGraphite)
            .padding(WindowInsets.safeDrawing.asPaddingValues())
            .padding(horizontal = 20.dp)
    ) {
        Spacer(Modifier.height(18.dp))
        Row(
            modifier = Modifier.fillMaxWidth(),
            verticalAlignment = Alignment.CenterVertically
        ) {
            Column(modifier = Modifier.weight(1f)) {
                Row(verticalAlignment = Alignment.CenterVertically) {
                    Box(
                        modifier = Modifier
                            .size(12.dp)
                            .background(NunYellow, CircleShape)
                    )
                    Spacer(Modifier.width(9.dp))
                    Text(
                        text = stringResource(R.string.brand_name),
                        color = NunYellow,
                        fontSize = 31.sp,
                        fontWeight = FontWeight.Black,
                        letterSpacing = 2.sp
                    )
                }
                Text(
                    text = stringResource(R.string.brand_subtitle),
                    color = NunSecondaryText,
                    fontSize = 10.sp,
                    letterSpacing = 1.5.sp
                )
            }
            IconButton(onClick = { scope.launch { reloadApps() } }) {
                Icon(Icons.Default.Refresh, contentDescription = stringResource(R.string.refresh_apps), tint = NunYellow)
            }
            IconButton(onClick = {
                runCatching { context.startActivity(Intent(Settings.ACTION_SETTINGS)) }
                    .onFailure { Toast.makeText(context, R.string.settings_unavailable, Toast.LENGTH_SHORT).show() }
            }) {
                Icon(Icons.Default.Settings, contentDescription = stringResource(R.string.open_settings), tint = NunText)
            }
        }

        Spacer(Modifier.height(22.dp))
        SearchField(
            value = query,
            onValueChange = { query = it },
            placeholder = stringResource(R.string.search_apps)
        )
        Spacer(Modifier.height(14.dp))

        Card(
            modifier = Modifier.fillMaxWidth(),
            colors = CardDefaults.cardColors(containerColor = NunPanel),
            shape = RoundedCornerShape(18.dp)
        ) {
            Row(
                modifier = Modifier.padding(horizontal = 16.dp, vertical = 14.dp),
                verticalAlignment = Alignment.CenterVertically
            ) {
                Column(modifier = Modifier.weight(1f)) {
                    Text(stringResource(R.string.local_device), color = NunSecondaryText, fontSize = 10.sp, letterSpacing = 1.sp)
                    Text(stringResource(R.string.launcher_ready), color = NunText, fontWeight = FontWeight.SemiBold, fontSize = 14.sp)
                }
                Box(
                    modifier = Modifier
                        .background(Color(0xFF26321B), RoundedCornerShape(50))
                        .padding(horizontal = 10.dp, vertical = 6.dp)
                ) {
                    Text(stringResource(R.string.local_only), color = NunYellow, fontSize = 10.sp, fontWeight = FontWeight.Bold)
                }
            }
        }

        Spacer(Modifier.height(22.dp))
        Row(verticalAlignment = Alignment.CenterVertically) {
            Text(stringResource(R.string.applications), color = NunText, fontWeight = FontWeight.Bold, fontSize = 18.sp)
            Spacer(Modifier.width(8.dp))
            Text(filtered.size.toString(), color = NunYellow, fontSize = 12.sp, fontWeight = FontWeight.Bold)
        }
        Spacer(Modifier.height(8.dp))

        when {
            loading -> CenterMessage(stringResource(R.string.loading_apps))
            filtered.isEmpty() -> CenterMessage(
                if (query.isBlank()) stringResource(R.string.no_apps)
                else stringResource(R.string.no_search_results)
            )
            else -> LazyVerticalGrid(
                columns = GridCells.Adaptive(minSize = 88.dp),
                modifier = Modifier.fillMaxSize(),
                contentPadding = PaddingValues(top = 8.dp, bottom = 28.dp),
                horizontalArrangement = Arrangement.spacedBy(10.dp),
                verticalArrangement = Arrangement.spacedBy(12.dp)
            ) {
                items(filtered, key = { it.activity.componentName.flattenToString() }) { app ->
                    AppTile(app = app) {
                        launchApp(context, app)
                    }
                }
            }
        }
    }
}

@Composable
private fun SearchField(
    value: String,
    onValueChange: (String) -> Unit,
    placeholder: String
) {
    Card(
        colors = CardDefaults.cardColors(containerColor = NunPanel),
        shape = RoundedCornerShape(14.dp)
    ) {
        Row(
            modifier = Modifier.fillMaxWidth().padding(horizontal = 14.dp, vertical = 13.dp),
            verticalAlignment = Alignment.CenterVertically
        ) {
            Icon(Icons.Default.Search, contentDescription = null, tint = NunYellow)
            Spacer(Modifier.width(10.dp))
            BasicTextField(
                value = value,
                onValueChange = onValueChange,
                modifier = Modifier.weight(1f),
                singleLine = true,
                cursorBrush = SolidColor(NunYellow),
                textStyle = MaterialTheme.typography.bodyLarge.copy(color = NunText),
                keyboardOptions = KeyboardOptions(imeAction = ImeAction.Search),
                keyboardActions = KeyboardActions(onSearch = {}),
                decorationBox = { inner ->
                    Box {
                        if (value.isEmpty()) Text(placeholder, color = NunSecondaryText)
                        inner()
                    }
                }
            )
        }
    }
}

@Composable
private fun AppTile(app: LaunchableApp, onClick: () -> Unit) {
    val bitmap = remember(app.icon) { drawableBitmap(app.icon).asImageBitmap() }
    Column(
        modifier = Modifier
            .fillMaxWidth()
            .background(NunPanel, RoundedCornerShape(18.dp))
            .clickable(onClick = onClick)
            .padding(horizontal = 8.dp, vertical = 13.dp),
        horizontalAlignment = Alignment.CenterHorizontally
    ) {
        Image(bitmap = bitmap, contentDescription = app.label, modifier = Modifier.size(42.dp))
        Spacer(Modifier.height(9.dp))
        Text(
            text = app.label,
            color = NunText,
            fontSize = 11.sp,
            lineHeight = 14.sp,
            maxLines = 2,
            fontWeight = FontWeight.Medium
        )
    }
}

@Composable
private fun CenterMessage(message: String) {
    Box(Modifier.fillMaxSize(), contentAlignment = Alignment.Center) {
        Text(message, color = NunSecondaryText, fontSize = 13.sp)
    }
}

private suspend fun queryLaunchableApps(context: Context): List<LaunchableApp> =
    runCatching {
        val launcherApps = context.getSystemService(LauncherApps::class.java)
            ?: return@runCatching emptyList()
        launcherApps.getActivityList(null, Process.myUserHandle())
            .mapNotNull { info ->
                val label = info.label?.toString()?.trim().orEmpty()
                if (label.isEmpty()) null else LaunchableApp(label, info, info.getIcon(0))
            }
            .sortedBy { it.label.lowercase() }
    }.getOrDefault(emptyList())

private fun launchApp(context: Context, app: LaunchableApp) {
    runCatching {
        val launcherApps = context.getSystemService(LauncherApps::class.java)
            ?: throw IllegalStateException("Launcher service unavailable")
        launcherApps.startMainActivity(app.activity.componentName, app.activity.user, null, null)
    }.onFailure { error ->
        val message = if (error is ActivityNotFoundException) R.string.app_unavailable else R.string.launch_failed
        Toast.makeText(context, message, Toast.LENGTH_SHORT).show()
    }
}

private fun drawableBitmap(drawable: Drawable): Bitmap {
    val bitmap = Bitmap.createBitmap(96, 96, Bitmap.Config.ARGB_8888)
    val canvas = Canvas(bitmap)
    val oldBounds = drawable.bounds
    drawable.setBounds(0, 0, 96, 96)
    drawable.draw(canvas)
    drawable.bounds = oldBounds
    return bitmap
}
