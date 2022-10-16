import sensor, image, time, math
from pyb import LED
from pyb import UART

sensor.reset()                         # Reset and initialize the sensor.
sensor.set_pixformat(sensor.GRAYSCALE) # Set pixel format to RGB565 (or GRAYSCALE)
sensor.set_framesize(sensor.QQVGA)     # Set frame size to QVGA (160x120)
sensor.skip_frames(time = 30)        # Wait for settings take effect.
sensor.set_auto_gain(False) # 关闭增益（色块识别时必须要关）
sensor.set_auto_whitebal(False) # 关闭白平衡
led = LED(2)
led.toggle()
led.on()


BLACK_THRESHOLD = [(0, 50, -24,-1, -18, 6)]#LAB阈值
clock = time.clock()
ROI_LinePatrol = [(0,0,110,20,0.7),(0,50,110,20,0.3),(0,100,110,20,0.1)]
#ROI_LinePatrol = [(0,0,160,40),(0,40,160,40),(0,80,160,40)]
ROI_LibraryAngleDetection = [(130,0,30,20),(130,50,30,20),(130,100,30,20)]
uart = UART(3, 115200) 
LibraryAngleFlag_1 = 0
LibraryAngleFlag_2 = 0
write_str_header = 'pp'
write_str_end = ',\r\n'
value_compensate = 35 # 补偿值
ROI_LinePatrolIndex = 0 # 获取元素在元组的位置
weight_sum = 0 #权值和初始化
for r in ROI_LinePatrol: weight_sum += r[4] # r[4] is the roi weight.计算权值和。遍历上面的三个矩形，r[4]即每个矩形的权值。

while(True):
    clock.tick()
    img = sensor.snapshot()
    img.draw_rectangle((0,0,110,20), color = (255,0,0))
    img.draw_rectangle((0,50,110,20), color = (123,137,0))
    img.draw_rectangle((0,100,110,20), color = (155,137,0))
    img.draw_rectangle(ROI_LibraryAngleDetection[0], color = (255,137,0))
    img.draw_rectangle(ROI_LibraryAngleDetection[1], color = (255,112,167))
    img.draw_rectangle(ROI_LibraryAngleDetection[2], color = (255,137,155))
    
    centroid_sum = 0  #利用颜色识别分别寻找三个矩形区域内的线段
    # center_pos = 0

    blobs_LibraryAngleDetection_0 = img.find_blobs(BLACK_THRESHOLD, roi=ROI_LibraryAngleDetection[0][0:4], merge=True) 
    if blobs_LibraryAngleDetection_0:
        LibraryAngleFlag_1 = 1
        largest_blob = max(blobs_LibraryAngleDetection_0, key=lambda b: b.pixels()) # 在blobs中找最大像素值
        img.draw_rectangle(largest_blob.rect())
        img.draw_cross(largest_blob.cx(),largest_blob.cy())
        # print('LibraryAngleFlag_1:'+str(LibraryAngleFlag_1))
    else:
        LibraryAngleFlag_1 = 0

    
    blobs_LibraryAngleDetection_2 = img.find_blobs(BLACK_THRESHOLD, roi=ROI_LibraryAngleDetection[2][0:4], merge=True)
    if blobs_LibraryAngleDetection_2:
        LibraryAngleFlag_2 = 1
        largest_blob = max(blobs_LibraryAngleDetection_2, key=lambda b: b.pixels()) # 在blobs中找最大像素值
        img.draw_rectangle(largest_blob.rect())
        img.draw_cross(largest_blob.cx(),largest_blob.cy())        
        # print('LibraryAngleFlag_2:'+str(LibraryAngleFlag_2))
    else:
        LibraryAngleFlag_2 = 0    
     

    for r in ROI_LinePatrol:         # 寻找黑线         
        blobs = img.find_blobs(BLACK_THRESHOLD, roi=r[0:4], merge=True) # r[0:4]是感兴趣区域的矩形元组.
        if blobs:
            most_pixels = 0
            # print("I find it!!!!")
            largest_blob = max(blobs, key=lambda b: b.pixels()) # 在blobs中找最大像素值
            img.draw_rectangle(largest_blob.rect())
            # 将此区域的像素数最大的颜色块画矩形和十字形标记出来
            img.draw_cross(largest_blob.cx(),largest_blob.cy())
            # print(largest_blob.cx())
            blobs_compensate = img.find_blobs(BLACK_THRESHOLD, roi=ROI_LibraryAngleDetection[ROI_LinePatrolIndex][0:4], merge=True)
            if blobs_compensate:
                centroid_sum += (largest_blob.cx()-value_compensate)* r[4]
            else:
                centroid_sum += largest_blob.cx()* r[4]
            # centroid_sum += largest_blob.cx() * r[4] # r[4] is the roi weight
            ROI_LinePatrolIndex += 1
    ROI_LinePatrolIndex = 0        
    if weight_sum:
        center_pos = (centroid_sum / weight_sum) # 加权均值中心x轴坐标
    deflection_angle = 0 
    ns = (center_pos-55)/60  # QQVGA 160x120.  roi 110x120 
    deflection_angle = -math.atan(ns)

    # deflection_angle = math.atan(ns)
    deflection_angle = math.degrees(deflection_angle)
    deflection_angle = int(deflection_angle+50)

    # print('cross:'+str(LibraryAngleFlag_1)+str(LibraryAngleFlag_2))
    uart.write(write_str_header+','+str(deflection_angle)+','+str(LibraryAngleFlag_1)+str(LibraryAngleFlag_2)+write_str_end) 
    # print(write_str_header+','+str(deflection_angle)+','+str(LibraryAngleFlag_1)+str(LibraryAngleFlag_2)+write_str_end)
    print("Turn Angle: %f" % deflection_angle)  
    # time.sleep(1)


    # uart.write(write_str_header+','+str(deflection_angle)+write_str_end)
    # print(write_str_header+','+str(deflection_angle)+write_str_end)
    
    # LibraryAngleDetectionCount = 0
    # for l in ROI_LibraryAngleDetection:
    #     blobs_LibraryAngleDetection = img.find_blobs(BLACK_THRESHOLD, roi=l[0:4], merge=True) 
    #     if blobs_LibraryAngleDetection:
    #         LibraryAngleDetectionCount += 1
    #         largest_blob = max(blobs_LibraryAngleDetection, key=lambda b: b.pixels()) # 在blobs中找最大像素值
    #         img.draw_rectangle(largest_blob.rect())
    # if LibraryAngleDetectionCount == 2:
    #     print("cross it")
    #     LibraryAngleDetectionCount = 0
    
    
  

    # if ((LibraryAngleFlag_1 == 1) and (LibraryAngleFlag_2 == 1)):
    #     if LibraryAngleFlag_1:
    #        print("none!")
    #     else:
    #         if LibraryAngleFlag_2:
    #             print("cross it \n")
    #             LibraryAngleFlag_1 = 0
    #             LibraryAngleFlag_2 = 0
           

        # if blobs_LibraryAngleDetection.count() == 1:
        #     blobs_LibraryAngleDetection = img.find_blobs(BLACK_THRESHOLD, roi=ROI_LibraryAngleDetection[0:4], merge=True)
        #     if blobs_LibraryAngleDetection.count() == 0:
                
        # if blobs:
        #     most_pixels = 0
        #     # print("I find it!!!!")
        #     largest_blob = max(blobs, key=lambda b: b.pixels()) # 在blobs中找最大像素值
        #     img.draw_rectangle(largest_blob.rect())
        #     # 将此区域的像素数最大的颜色块画矩形和十字形标记出来
        #     img.draw_cross(largest_blob.cx(),largest_blob.cy())
    



