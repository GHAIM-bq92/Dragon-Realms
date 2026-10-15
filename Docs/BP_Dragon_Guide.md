دليل تفصيلي: إنشاء BP_Dragon داخل Unreal Editor 5.3

الهدف: إنشاء Blueprint لتنّين قابل للركوب مرتبط بالمكونات البرمجية (DragonMountActor و MountComponent) بحيث يمكن للاعب الاقتراب والركوب والتحكم في التنين.

ملاحظات قبل البدء:
- تأكد أن بنيت المشروع (Build) في Visual Studio وأنك تفتح المشروع في Unreal Editor 5.3.
- لا أستطيع رفع ملفات .uasset أو FBX محمية؛ استعمل نموذجًا مجانيًا من Marketplace أو أصولك الخاصة.

الخطوات:
1. إنشاء Blueprint
   - Content Browser → Create → Blueprint Class → اختر DragonMountActor كـ Parent.
   - سمّه: BP_Dragon.

2. إعداد المكوّن Skeletal Mesh
   - افتح BP_Dragon → في Components ترى SkeletalMeshComp.
   - اختَر Skeletal Mesh مناسب (مثال: أي نموذج جناحي من Marketplace أو نموذج "flying creature").
   - افتح الـSkeletal Mesh في Asset Editor → على عظمة الظهر أو العمود الفقري أنشئ Socket جديد:
     - اسم Socket: RiderSocket (أو نفس الاسم الموجود في RiderSocketName في الكود).
     - اضبط الـTransform (موقع/دوران) بحيث يكون الراكب جالسًا على الظهر بشكل طبيعي.

3. ربط AnimBlueprint
   - إذا أنشأت AnimBlueprint (انظر الملف الخاص بالأنيميشن أدناه) فعيّنها في SkeletalMeshComp → Animation → Anim Class.

4. تنفيذ أحداث Mount/Unmount داخل BP_Dragon (Blueprint Event Graph)
   - أضف Custom Event اسمه OnMounted (أو استعمل Event dispatcher إن أردت الاتصال من C++ Multicast).
   - من OnMounted نفّذ Attachment:
     - Input: Actor Rider.
     - استخدم AttachToComponent: Target = Rider (أو استخدم AttachActorToComponent) بحيث Attach the Rider to SkeletalMeshComp with Socket = RiderSocket.
     - ضبط قواعد التثبيت: SnapToTargetNotIncludingScale.
   - أضف Custom Event اسمه OnDismounted يقوم بعمل DetachFromActor وتهيئة حاله اللاعب (إعادة التمكين والظهور).

5. التعامل مع VFX وواصفات المقعد
   - أضف SceneComponent فارغًا ك�� RiderSeatMarker في الموقع الدقيق داخل SkeletalMesh لتكون مرجعًا في حالة الحاجة.
   - أضف ParticleSystem أو Niagara System للأصوات/VFX عند Mount/Unmount (اختياري).

6. ضبط Collision وInteraction
   - اجعل SkeletalMeshComp أو Root Component يمتلك Collision Profile يسمح بالخط التصادمي (line trace) من Pawn للاختبار.
   - Player Pawn يستخدم سطر الرؤية (line trace) لمسافة ~500 وحدات ويستدعي Server_RequestMountOnTarget في الكود الحالي.

7. اختبار محلي
   - ضع BP_Dragon على الخريطة (اختبر في منطقة مفتوحة بمساحة كافية للطيران).
   - Play → New Editor Window (PIE) → Number of Players = 2.
   - في نافذة العميل، صوب التنين واضغط F (Interact). لاحظ إن كان الراكب يلتصق ويختفي الـPawn القديم (في حالة possession transfer).

ملاحظات إضافية
- إذا رغبت أن لا يتم تحوّل السيطرة (possession transfer) وأردت حلًا أبسط: قم بتعطيل منطق Possession في MountComponent وبدلًا عنه نفّذ AttachOnly (أي اللاعب يظل نفس الـPawn لكنه يكون attached وhidden). هذا يقلل التعقيد الشبكي لكنه يجعل من الصعب إدارة التحكم بسهولة.


---

دليل تفصيلي: إنشاء AnimBlueprint للتنين (مخطط منطقي)

الهدف: AnimBlueprint مبسّط يعتمد على UDragonAnimInstance (Speed, bIsFlying) ليشغّل حالات Idle / Flap / Glide / Takeoff / Land.

الخطوات:
1. إنشاء AnimBlueprint
   - Content → Create → Animation → Animation Blueprint.
   - Parent Class: DragonAnimInstance (المضاف في C++).
   - سمّه: ABP_DragonAnim.

2. إعداد المتغيرات
   - Speed (float) — يُزوَّد تلقائيًا من AnimInstance.
   - bIsFlying (bool) — يُزوَّد تلقائيًا.
   - VerticalSpeed (float) — يمكنك إضافته إذا أردت دقة أكبر (حساب من velocity.Z عبر Blueprint).

3. AnimGraph (اختصار منطقي)
   - ابني State Machine مكوّن من States: Idle, Takeoff, Flap, Glide, Land.
   - اقتراح الانتقالات (Transitions):
     - Idle -> Takeoff: إذا bIsFlying == true && Speed > smallThreshold && VerticalSpeed > 50
     - Takeoff -> Flap: بعد انتهاء montion/anim أو إذا Speed > threshold
     - Flap -> Glide: إذا Speed > flapSpeedThreshold && !IsAccelerating الجناح (منطق مخصّص)
     - Glide -> Flap: إذا玩家 ضغط Boost أو Speed زادت
     - Any -> Land: إذا bIsFlying == false && IsNearGround (يمكن حسابه عبر LineTrace من AnimBP أو إشارة من BP_Dragon)
     - Land -> Idle: بعد انتهاء Land animation

4. Blendspaces (اختياري)
   - لإنشاء حركة انتقالية سلسة بين Flap وGlide استعمل 1D BlendSpace على محور السرعة أو زاوية الميل (Pitch).

5. Replication considerations
   - AnimBlueprint يعمل محليًا على كل عميل — ما يحتاج replicate. لكن حالات الدخول (بداية Takeoff/Flap) يجب أن تُعلَن عبر Multicast أو أن تكون مشتقة من الحركة المكرّرة (Movement replication).
   - أفضل ممارسة: الخادم يحدّث حالات gameplay (bIsFlying, speed)، ويجعلها مُكرّرة أو يرسل أحداث multicast؛ AnimBP تقرأ هذه المتغيرات لتطبيق الأنيميشن محليًا.


---

خريطة اختبار: خطوات سريعة لبناء مشهد اختبار صغير

الهدف: عمل خريطة صغيرة قابلة لاختبار الطيران، الهبوط، والركوب.

المكونات المطلوبة:
- Terrain بسيط (Landmass)
- Mountain: استخدم Landscape sculpt أو Modular Mountains من Marketplace
- Forest: Scatter بعض الأشجار من Foliage tool
- Lake: استخدم Water plugin لتشكيل بحيرة
- Floating Island: اصنع Mesh بسيط أو استخدم Static Mesh من Marketplace وضعه في السماء
- Castle ruin: ضع بعض Modular pieces لعمل خراب صغير

الخطوات:
1. New Level → Basic (Empty level)
2. Landscape: Create → sculpt mountain, create slope paths
3. Place lake at base of mountain (Water Body Lake)
4. Create floating island: اسحب Static Mesh، أضف collision بسيط
5. تنسيق الإضاءة: Directional Light (sun), SkyAtmosphere, Exponential Height Fog
6. وضع BP_Dragon: في منطقة جبلية أو على الجزيرة الطائرة
7. Spawn Points: ضع PlayerStart وPlayerStart_Second
8. NavMesh: أضف NavMeshBoundsVolume إذا أردت اختبار AI لاحقًا

اختبار الخريطة:
- Play as 2 players (PIE) → تأكّد أن الطيران ممكن وأن الهبوط على الجزيرة الطائرة يعمل.


---

ما الذي سأفعله تالياً إذا وافقت (بناء تلقائي للـBlueprints غير ممكن بدون أصول):
- أقدّم ملف تعليمات مفصل خطوة بخطوة (قابل للنسخ واللصق) لكل خطوة داخل Editor — يسهل عليك تنفيذها بنفسك.
- أجهّز ملفات README إضافية لكل مهمة وأدرج سكربت اختبار بسيط (أوامر تشغيل Dedicated Server للـUE5). 
- أساعدك في ربط AnimBlueprint بالـMountComponent (event hooks) عبر أمثلة Blueprints نصية.

أجب "نعم" لأضيف ملفات README التفصيلية إلى المستودع، أو أخبرني أي جزء تريد أن أبدأ به (مثال: "ابدأ AnimBlueprint" أو "ابدأ خريطة اختبار").