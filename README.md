# FramerateNotMatchModel
跟踪仪和扫描仪帧率不匹配导致组图帧率低问题建模

基于生产者消费者模型
Producer：Scanner,Tracker
Consumer: SnSyncService

模拟由于带宽等原因导致的跟踪仪和扫描仪帧率相差在10以上，导致最终的组图帧率很低的问题
