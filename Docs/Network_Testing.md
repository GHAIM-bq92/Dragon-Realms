# خطوات إضافية لاختبار الشبكة (LAN) وتشغيل Dedicated Server محليًا

1) تشغيل نسخة خادم محليًا من Editor (Play as Listen Server)
   - Editor → Play → Advanced Settings → Enable "Run Dedicated Server" أو شغّل كمستضيف (Listen server) عبر Play → New Editor Window (PIE) مع خيار Number of Players > 1.

2) تشغيل Dedicated Server مبني (Packaged Server)
   - قم بإنشاء Build لخادم مخصص: File → Package Project → Build Configuration: Development Server (أو Shipping Server) → هدف Windows.
   - شغّل الملف التنفيذي للخادم على جهازك أو جهاز آخر في الشبكة.
   - اجعل العملاء ينضمون عبر الأمر: open <server_ip>:<port> في نافذة الـConsole أو في Launch URL.

3) اختبار التكرار
   - عند الركوب: تحقق على العميل الآخر أن Rider يظهر في الموضع الصحيح وAnimation state تعمل.
   - راقب netprofiler وStat Replication في Unreal لاتّخاذ إجراءات تحسين.

4) ملاحظات للـpossession transfer
   - عند نقل السيطرة (possess) يجب اختبار الخاصيات التالية:
     - هل تم استعادة الـController بنجاح عند Dismount؟
     - هل الـPawn القديم لا يزال مرئيًا أو له تأثيرات فيزيائية؛ إذا كان مخفيًا فهل سيظل في مكانه الصحيح؟
     - هل توجد حالات edge-case (انقطاع الشبكة أثناء الركوب) — اكتب سيناريوهات للاختبار.


---

ملخص
- رفعت ملفات إرشادية مفصلة (Docs/BP_Dragon_Guide.md) تشرح إنشاء BP_Dragon وAnimBlueprint وخريطة اختبار وكيفية اختبار الشبكة محلياً.
- إذا أردت، أستطيع الآن إضافة README مُنسّق في root أو سكربتات CI لبناء خادم. 

أجب "نعم" لأدخل ملفات README التفصيلية في الـrepo (إن لم تُنفَّذ بالفعل) أو اطلب مباشرة أي ملف / مثال Blueprints نصي آخر.