# Beelzebub × Satan — архітектура інтеграції

> Статус: **архітектурна специфікація / proposal**. Це не доказ реалізації та не production certification.

## 1. Модель

- **Beelzebub — Assurance & Defense Plane:** перевірка цілісності, конфігурацій, provenance, доказів, тестів і відхилень; пояснення ризиків та рекомендації.
- **Satan — Coordination & Recovery Plane:** інвентаризація, міжрепозиторні контракти, аудит, звіти, планування контрольованих змін і release readiness.

Це технічні кодові назви. Вони не означають автономний інтелект, безпомилковий захист або право на самовільні зміни.

## 2. Історична точність

Сатана походить від семітського слова зі значенням «противник/обвинувач»; його біблійний образ і пізніша персоніфікація розвивалися історично. Ототожнення Сатани з Люцифером/Денницею залежить від конкретної традиції та тлумачення.

Веельзевул пов’язаний із біблійною формою Баал-Зевув і давніми левантійськими культами. «Владика мух» — відома полемічна інтерпретація; співвідношення Baal-zebub / Baal-zebul та етимологія не зводяться до безспірної формули. Титули «князь демонів», «міністр пекла» й «радник Сатани» належать різним пізнішим демонологічним і літературним традиціям.

У технічній метафорі: **Beelzebub = виявлення деградації та assurance; Satan = координація, аудит і контрольоване відновлення.**

## 3. Локальний профіль NUNLauncher

Host-side boot-image admission and validation. Beelzebub verifies manifest schema, hashes, bounds, metadata and evidence; Satan reports boot-artifact compatibility across repositories. Validation must remain non-executing and must not claim publisher authenticity unless a real signature chain is verified.

Спільна модель не створює спільного runtime, не скасовує локальні API та не змінює межі довіри проєкту.

## 4. Контур Beelzebub

1. **Preflight Inspector:** перевірка конфігурацій, manifests, залежностей і передумов.
2. **Build Evidence:** фіксація commit SHA, toolchain, параметрів, тестів і provenance артефактів.
3. **Integrity Monitor:** контроль очікуваних файлів, хешів і підписів, якщо підписання реально налаштоване.
4. **Policy & Threat Memory:** версіоновані правила й локальна історія відхилень без збирання секретів.
5. **Target Guardian:** лише перевірки, підтримані реальною платформою; host evidence не підміняє target qualification.
6. **Recovery Advisor:** найменший рекомендований крок із доказами; без автоматичного застосування.

## 5. Контур Satan

1. **Repository Registry:** репозиторії, default branch, власники контрактів і статус інтеграції.
2. **Contract Graph:** версіоновані API, manifests, формати артефактів і сумісність.
3. **Audit Orchestrator:** тільки явно дозволені перевірки.
4. **Change Planner:** ізольовані feature branches і PR.
5. **Recovery Ledger:** журнал сигналу, доказів, рішення, автора й результату.
6. **Release Coordinator:** release-readiness звіт без автоматичного merge/deploy.

## 6. Контракт доказів

Кожен результат містить `schema_version`, `repository`, `commit_sha`, `timestamp_utc`, `check_id`, `status`, `evidence_refs`, `tool_versions`, `policy_version`, `severity`, `explanation` і `recommended_action`.

Стани: `pass`, `fail`, `unknown`, `not_applicable`. Відсутність доказу — `unknown`, не pass/fail. Будь-який score повинен мати опубліковану методику й перелік непокритих перевірок.

## 7. Безпека й обмеження

- Read-only, least privilege та fail-safe за замовчуванням.
- Не збирати й не логувати секрети, приватні ключі, токени, особисті дані або backup-вміст.
- Не виконувати код сусіднього репозиторію через сам факт його реєстрації.
- Не змінювати `main`, release tags, production, firmware або hardware автоматично.
- Кожна зміна — окрема гілка, мінімальний diff, тести, audit trail і PR.
- P2P, підписи, SBOM, sandbox, threat intelligence й auto-healing — **planned**, доки реалізацію не доведено кодом і тестами.
- Рекомендація не є merge gate; обов’язкові gates задаються явними repository rules.

## 8. Впровадження поетапно

**0 — Inventory:** точки входу build/test, активи, залежності та власники контрактів.

**1 — Observe:** read-only перевірки, fixtures і структуровані звіти.

**2 — Verify:** schema validation, позитивні/негативні тести, regression і reproducibility.

**3 — Coordinate:** версіоновані контракти та агрегування через Hellduck/Satan Nexus без виконання чужого коду.

**4 — Controlled remediation:** лише після окремого схвалення — patch branch/PR; без автоматичного merge/deploy.

## 9. Критерії готовності

- Локальна зона відповідальності та trust boundary задокументовані.
- Перевірки мають стабільні ID, версії та зрозумілі результати.
- `unknown` відокремлений від `pass`.
- Є позитивні, негативні й regression-тести.
- Секрети відсутні у звітах.
- Документація відповідає коду; planned не називається implemented.
- Зміни проходять через PR, рішення про merge залишається за maintainer.

## 10. Принцип

**Beelzebub виявляє й пояснює деградацію. Satan пов’язує докази, контракти та план дій. Людина з відповідними повноваженнями затверджує зміни.**
