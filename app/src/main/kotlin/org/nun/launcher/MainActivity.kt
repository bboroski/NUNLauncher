package org.nun.launcher

import android.app.Activity
import android.content.ActivityNotFoundException
import android.content.ComponentName
import android.content.Context
import android.content.pm.LauncherApps
import android.graphics.Color
import android.graphics.Rect
import android.graphics.Typeface
import android.graphics.drawable.Drawable
import android.graphics.drawable.GradientDrawable
import android.os.Bundle
import android.view.Gravity
import android.view.View
import android.view.ViewGroup
import android.widget.EditText
import android.widget.GridLayout
import android.widget.LinearLayout
import android.widget.ScrollView
import android.widget.TextView
import android.widget.Toast
import android.text.Editable
import android.text.TextWatcher
import java.text.Collator
import java.util.Locale

class MainActivity : Activity() {
    private data class LaunchItem(
        val title: String,
        val component: ComponentName,
        val user: android.os.UserHandle,
        val icon: Drawable
    ) {
        val stableKey: String
            get() = user.hashCode().toString() + ":" + component.flattenToString()
    }

    private val launcherApps: LauncherApps by lazy {
        getSystemService(Context.LAUNCHER_APPS_SERVICE) as LauncherApps
    }
    private val preferences by lazy {
        getSharedPreferences("nunlauncher.home", Context.MODE_PRIVATE)
    }
    private val favoriteKeys = linkedSetOf<String>()
    private var allApps: List<LaunchItem> = emptyList()

    private lateinit var appGrid: GridLayout
    private lateinit var searchField: EditText
    private lateinit var countLabel: TextView
    private lateinit var emptyLabel: TextView
    private lateinit var statusLabel: TextView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        window.statusBarColor = Color.rgb(13, 13, 13)
        window.navigationBarColor = Color.rgb(13, 13, 13)

        favoriteKeys.addAll(preferences.getStringSet("favorites", emptySet()) ?: emptySet())
        buildHome()
        refreshApps()
    }

    override fun onResume() {
        super.onResume()
        if (::appGrid.isInitialized) refreshApps()
    }

    private fun buildHome() {
        val root = LinearLayout(this).apply {
            orientation = LinearLayout.VERTICAL
            setPadding(dp(18), dp(22), dp(18), dp(12))
            setBackgroundColor(Color.rgb(13, 13, 13))
        }

        val brand = TextView(this).apply {
            text = getString(R.string.brand_title)
            setTextColor(Color.rgb(255, 214, 0))
            textSize = 30f
            typeface = Typeface.create("sans-serif", Typeface.BOLD)
        }
        root.addView(brand, matchWrap())

        val subtitle = TextView(this).apply {
            text = getString(R.string.brand_subtitle)
            setTextColor(Color.rgb(153, 153, 153))
            textSize = 11f
            letterSpacing = 0.12f
            setPadding(0, 0, 0, dp(18))
        }
        root.addView(subtitle, matchWrap())

        searchField = EditText(this).apply {
            hint = getString(R.string.search_hint)
            setSingleLine(true)
            textSize = 16f
            setTextColor(Color.rgb(230, 230, 230))
            setHintTextColor(Color.rgb(120, 120, 120))
            setPadding(dp(16), dp(12), dp(16), dp(12))
            background = roundedBackground(Color.rgb(26, 26, 26), Color.rgb(55, 55, 55))
            contentDescription = getString(R.string.search_accessibility)
        }
        root.addView(
            searchField,
            LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, dp(52))
        )

        countLabel = TextView(this).apply {
            setTextColor(Color.rgb(191, 161, 0))
            textSize = 12f
            setPadding(dp(2), dp(16), dp(2), dp(8))
        }
        root.addView(countLabel, matchWrap())

        appGrid = GridLayout(this).apply {
            columnCount = calculateColumnCount()
            alignmentMode = GridLayout.ALIGN_BOUNDS
            useDefaultMargins = false
        }
        emptyLabel = TextView(this).apply {
            setTextColor(Color.rgb(153, 153, 153))
            textSize = 14f
            gravity = Gravity.CENTER
            setPadding(dp(20), dp(44), dp(20), dp(44))
            visibility = View.GONE
        }

        val scroll = ScrollView(this).apply {
            isFillViewport = true
            addView(LinearLayout(this@MainActivity).apply {
                orientation = LinearLayout.VERTICAL
                addView(appGrid, matchWrap())
                addView(emptyLabel, matchWrap())
            })
        }
        root.addView(
            scroll,
            LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, 0, 1f)
        )

        statusLabel = TextView(this).apply {
            text = getString(R.string.core_disconnected)
            setTextColor(Color.rgb(120, 120, 120))
            textSize = 11f
            setPadding(0, dp(12), 0, dp(4))
        }
        root.addView(statusLabel, matchWrap())

        searchField.addTextChangedListener(object : TextWatcher {
            override fun beforeTextChanged(s: CharSequence?, start: Int, count: Int, after: Int) = Unit
            override fun onTextChanged(s: CharSequence?, start: Int, before: Int, count: Int) {
                renderApps(s?.toString().orEmpty())
            }
            override fun afterTextChanged(s: Editable?) = Unit
        })

        setContentView(root)
    }

    private fun refreshApps() {
        try {
            val discovered = launcherApps.profiles.flatMap { profile ->
                launcherApps.getActivityList(null, profile).map { info ->
                    LaunchItem(
                        title = info.label?.toString()?.takeIf { it.isNotBlank() }
                            ?: info.componentName.packageName,
                        component = info.componentName,
                        user = info.user,
                        icon = info.getBadgedIcon(0)
                    )
                }
            }

            val collator = Collator.getInstance(Locale.getDefault())
            allApps = discovered
                .distinctBy { it.stableKey }
                .sortedWith { left, right -> collator.compare(left.title, right.title) }

            statusLabel.text = getString(R.string.core_disconnected)
            renderApps(searchField.text?.toString().orEmpty())
        } catch (_: SecurityException) {
            allApps = emptyList()
            statusLabel.text = getString(R.string.app_discovery_restricted)
            renderApps(searchField.text?.toString().orEmpty())
        } catch (_: RuntimeException) {
            allApps = emptyList()
            statusLabel.text = getString(R.string.app_discovery_failed)
            renderApps(searchField.text?.toString().orEmpty())
        }
    }

    private fun renderApps(query: String) {
        if (!::appGrid.isInitialized) return

        val normalizedQuery = query.trim().lowercase(Locale.getDefault())
        val filtered = allApps
            .filter {
                normalizedQuery.isEmpty() ||
                    it.title.lowercase(Locale.getDefault()).contains(normalizedQuery)
            }
            .sortedWith(
                compareByDescending<LaunchItem> { favoriteKeys.contains(it.stableKey) }
                    .thenBy { it.title.lowercase(Locale.getDefault()) }
            )

        appGrid.removeAllViews()
        filtered.forEach { item ->
            val tile = TextView(this).apply {
                text = item.title
                setTextColor(Color.rgb(230, 230, 230))
                textSize = 12f
                gravity = Gravity.CENTER
                maxLines = 2
                ellipsize = android.text.TextUtils.TruncateAt.END
                compoundDrawablePadding = dp(8)
                setPadding(dp(5), dp(10), dp(5), dp(10))
                setCompoundDrawablesWithIntrinsicBounds(null, item.icon, null, null)
                background = roundedBackground(Color.rgb(26, 26, 26), Color.rgb(42, 42, 42))
                isClickable = true
                isFocusable = true
                contentDescription = getString(R.string.launch_app_accessibility, item.title)
                setOnClickListener { launch(item) }
                setOnLongClickListener {
                    toggleFavorite(item)
                    true
                }
            }

            val params = GridLayout.LayoutParams(
                GridLayout.spec(GridLayout.UNDEFINED),
                GridLayout.spec(GridLayout.UNDEFINED, 1f)
            ).apply {
                width = 0
                height = dp(112)
                setMargins(dp(4), dp(4), dp(4), dp(4))
            }
            appGrid.addView(tile, params)
        }

        countLabel.text = getString(R.string.app_count, filtered.size)
        emptyLabel.text = if (allApps.isEmpty()) {
            getString(R.string.no_apps_available)
        } else {
            getString(R.string.no_search_matches)
        }
        emptyLabel.visibility = if (filtered.isEmpty()) View.VISIBLE else View.GONE
    }

    private fun launch(item: LaunchItem) {
        try {
            launcherApps.startMainActivity(item.component, item.user, Rect(), Bundle())
        } catch (_: ActivityNotFoundException) {
            Toast.makeText(this, R.string.app_no_longer_available, Toast.LENGTH_SHORT).show()
            refreshApps()
        } catch (_: SecurityException) {
            Toast.makeText(this, R.string.app_launch_denied, Toast.LENGTH_SHORT).show()
        } catch (_: RuntimeException) {
            Toast.makeText(this, R.string.app_launch_failed, Toast.LENGTH_SHORT).show()
        }
    }

    private fun toggleFavorite(item: LaunchItem) {
        val added = if (favoriteKeys.contains(item.stableKey)) {
            favoriteKeys.remove(item.stableKey)
            false
        } else {
            favoriteKeys.add(item.stableKey)
            true
        }
        preferences.edit().putStringSet("favorites", favoriteKeys.toSet()).apply()
        Toast.makeText(
            this,
            if (added) R.string.favorite_added else R.string.favorite_removed,
            Toast.LENGTH_SHORT
        ).show()
        renderApps(searchField.text?.toString().orEmpty())
    }

    private fun roundedBackground(fill: Int, stroke: Int): GradientDrawable {
        return GradientDrawable().apply {
            shape = GradientDrawable.RECTANGLE
            cornerRadius = dp(15).toFloat()
            setColor(fill)
            setStroke(dp(1), stroke)
        }
    }

    private fun calculateColumnCount(): Int =
        (resources.configuration.screenWidthDp / 104).coerceIn(3, 6)

    private fun matchWrap() = LinearLayout.LayoutParams(
        ViewGroup.LayoutParams.MATCH_PARENT,
        ViewGroup.LayoutParams.WRAP_CONTENT
    )

    private fun dp(value: Int): Int =
        (value * resources.displayMetrics.density).toInt()
}
