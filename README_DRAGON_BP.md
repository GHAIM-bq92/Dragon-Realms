### ملاحظة: التوصيل إلى Socket عند الركوب

تم تحديث MountComponent ليقوم الآن بمحاولة إرفاق الراكب مباشرةً إلى SkeletalMeshComponent وSocket المعرّف في `DragonMountActor::RiderSocketName` عندما يكون المالك تنينًا (`ADragonMountActor`). هذا يجعل وضع الراكب مضبوطًا بشكل أدق بدلاً من الاعتماد على AttachToActor العام.

إذا استخدمت `BP_Dragon` كما في README_DRAGON_BP.md واجهت سلوك الإرفاق الآلي هذا — تأكد من أن لديك Socket باسم `RiderSocket` على الـ Skeletal Mesh لكي يعمل بشكل صحيح.
