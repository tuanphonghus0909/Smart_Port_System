# Smart_Port_System
Dự án lần này được phát triển dựa trên dự án cũ Giám sát và Cảnh báo Mực Nước Khai Thác tại Cảng Biển. Dưới cái nhìn thực tế từ địa phương nơi em sống, dự án lần này sẽ giải quyết 1 số vấn đề cơ bản của cảng biển hiện nay.

### Đặt bài toán
Cảng biển là cấu nối quan trọng trong hạng hải, thương mại và logistics toàn cầu. Chúng không chỉ là nơi trao đổi hàng hóa quốc tế, mà còn đóng vai trò quan trọng trong chuỗi cung ứng kinh tế. Tuy nhiên, với sự gia tăng nhanh chóng về khối lượng hàng hóa, số lượng tàu thuyền, và lưu lượng xe tải, nhiều cảng biển đang đối mặt với những thách thức lớn:

#### Đảm bảo an toàn cho tàu thuyền 

An toàn là yếu tố hàng đầu trong hoạt động cảng biển, đặc biệt đối với tàu thuyền lớn. Mỗi một đường chềo trong cảng cần đạt độ sâu nước tối ưu để tránh nguy cơ tàu mắc cạn hay tai nạn nghiêm trọng. Tuy nhiên, việc giám sát mực nước bên trong này thường chưa được đầu tư một cách hệ thống. Theo báo cáo, đã có nhiều sự cố do mực nước không đủ độ sâu yêu cầu khi tàu cập cảng.

#### Tình trạng ùn tắc giao thông tại cửa cảng 

Đối mặt khác, cổng và cửa ra vào của cảng thường là điểm nóng giao thông. Số lượng lớn xe tải, xe cố, và các loại phương tiện khác trong quá trình làm hàng thường gây ùn tắc nghiêm trọng. Cảnh tình này không chỉ làm giảm hiệu suất hoạt động của cảng, mà còn góp phần gia tăng phát thải do các xe phải chờ đợi. Đặc biệt, các khu dân cư quanh cảng thường xuyên chịu ảnh hưởng do ùn tắc giao thông này gây ra.

#### Quản lý bãi đỗ xe trong cảng

Bãi đỗ xe là yếu tố thiết yếu để quản lý hiệu quả đường đi và điều hành giao thông trong cảng. Nhưng do hạ tầng chưa được cải thiện toàn diện, nhiều xe phải loay hoay tìm chỗ, điều này tốn thời gian và gây trì trệ hội luu giao thông trong cảng.

#### Tuân thủ quy định về tải trọng hàng hóa

Khâu kiểm tra tải trọng là bắt buộc đối với mỗi xe xuất cảng. Việc quá tải không chỉ vi phạm quy định pháp luật, mà còn gây mất an toàn cho đường bộ. Theo báo cáo từ các cơ quan chức năng, nhiều vụ tai nạn giao thông nghiêm trọng trên các tuyến quốc lộ xuất phát từ nguyên nhân xe chở hàng quá tải trọng. Ngoài ra, việc không tuân thủ tải trọng cho phép còn làm hư hỏng đường sá, tăng chi phí bảo trì hạ tầng giao thông quốc gia. Tuy nhiên, trong thực tế, việc kiểm tra tải trọng tại các cảng biển vẫn còn nhiều bất cập do quy trình thủ công, thiếu thiết bị đo lường hiện đại và sự quản lý chặt chẽ.

### Nội dung chính

Từ vấn đề trên, việc triển khai các cảm biến đo lường, hệ thống giám sát tự động và giải pháp IoT (Internet of Things) sẽ giúp tối ưu hóa hoạt động cảng biển. Không chỉ giảm thiểu rủi ro tai nạn và ùn tắc, các hệ thống thông minh còn mang lại tính minh bạch, cải thiện hiệu suất vận hành, và tăng cường sự hài lòng cho các bên liên quan. Hệ thống được thiết kế với các chức năng như sau:
#### 1. **Tự Động Kiểm Tra Trọng Lượng Xe**
- ESP32 đọc dữ liệu từ cảm biến Loadcell và hiển thị kết quả trên màn hình OLED.
- Cảnh báo quá tải qua đèn LED RGB.
- Gửi dữ liệu tải trọng qua giao thức MQTT.

#### 2. **Quản Lý Ra Vào Cổng**
- ESP32 điều khiển động cơ servo tự động đóng/mở cổng dựa trên tín hiệu từ cảm biến hồng ngoại.
- Ghi nhận số lượng xe vào/ra và gửi dữ liệu lên hệ thống qua giao thức MQTT.

#### 3. **Giám Sát Bãi Đỗ Xe**
- ESP32 sử dụng cảm biến siêu âm để xác định số lượng chỗ đỗ xe trống.
- Hiển thị thông tin bãi đỗ xe trên màn hình OLED.
- Cập nhật trạng thái bãi đỗ qua giao thức MQTT.

#### 4. **Giám Sát Và Điều Phối Mực Nước**
- ESP32 đọc dữ liệu từ cảm biến mực nước và gửi thông tin về hệ thống qua MQTT.
- Điều phối tàu thông qua động cơ servo khi mức nước đạt điều kiện an toàn.

---

## Các Tính Năng Nâng Cao
- **ESP32 Với Giao Thức MQTT**: Gửi và nhận dữ liệu từ các thành phần trong hệ thống qua kết nối Wi-Fi bảo mật.
- **Ticker**: Thực hiện các tác vụ định kỳ như kiểm tra trọng lượng, giám sát mực nước và bãi đỗ.
- **Hiển Thị Song Song**: Sử dụng hai màn hình OLED để hiển thị thông tin bãi đỗ và trọng lượng xe.

---
### Chi tiết thiết kế
#### 1. **Hệ Thống Kiểm Tra Tải Trọng Xe**
- **Cảm Biến Loadcell**: Đo trọng lượng của xe trước khi xuất cảng.
- **Màn Hình OLED**: Hiển thị trọng lượng của xe theo thời gian thực.
- **LED RGB**: Cảnh báo khi xe quá tải. 
  - **Màu xanh**: Bình thường.
  - **Màu đỏ**: Quá tải.

#### 2. **Hệ Thống Quản Lý Ra Vào Cảng**
- **ESP32**: Kết nối và điều phối các cảm biến và động cơ servo.
- **Servo Motor**: 
  - 1 servo để điều phối tàu.
  - 2 servo để điều khiển cổng ra và vào tại bến cảng.
- **Cảm Biến Hồng Ngoại (TCRT5000)**: 
  - Phát hiện xe ra vào cảng thông qua tín hiệu analog từ cảm biến.
  - Gửi dữ liệu xe vào/ra về hệ thống quản lý.

#### 3. **Hệ Thống Quản Lý Bãi Đỗ Xe**
- **Cảm Biến Siêu Âm (HC-SR04)**: 
  - 3 cảm biến đo khoảng cách để phát hiện các chỗ đỗ xe còn trống.
  - Xác định số lượng chỗ đỗ còn lại.
- **Màn Hình OLED**: Hiển thị số lượng chỗ đỗ xe trống.
- **ESP32**: Thu thập và xử lý dữ liệu từ các cảm biến siêu âm.

#### 4. **Hệ Thống Giám Sát Mực Nước**
- **Cảm Biến Mực Nước**: Đo mức nước khai thác để đảm bảo an toàn khi tàu cập cảng.
- **Servo Motor**: Điều phối tàu dựa trên kết quả đo mực nước.
- **ESP32**: Xử lý dữ liệu từ cảm biến mực nước và điều khiển động cơ servo.

#### 5. **Mô Hình Hệ Thống Bến Cảng**
- Hệ thống bến cảng được thiết kế trên phần mềm Autodesk Fusion 360 để phục vụ in 3D. Dưới đây là hình ảnh thiết kế mô hình:
![image](https://github.com/user-attachments/assets/a9a74a09-af38-4567-9869-24c5d10c3f6f)

![image](https://github.com/user-attachments/assets/547df133-8ee1-4450-a3dc-7d85c1f1c244)

![image](https://github.com/user-attachments/assets/a5bb9b2c-2a35-4c4e-ab91-cf3d74d468e3)

---
### Mục tiêu đề tài

- Tự động hóa quy trình kiểm tra tải trọng và cảnh báo quá tải.

- Nâng cao hiệu quả và an toàn trong quản lý ra vào cảng.

- Tối ưu hóa việc sử dụng bãi đỗ xe bằng cách hiển thị thông tin chỗ trống.

- Đảm bảo an toàn hoạt động khai thác với hệ thống giám sát mực nước.
### Sơ đồ khối

![CKIOT drawio](https://github.com/user-attachments/assets/7d4121c8-ab81-4efe-a104-7783ee9f4f2e)

### Kết quả

Các kết quả được thể hiện trong link drive sau: https://drive.google.com/drive/folders/1L92neevsPCF2J952qQxhsg_2pOWCFQuE?lfhs=2

### Kết luận
Dự án Smart Port System đã được xây dựng và triển khai ở mức mô phỏng, tập trung vào giải quyết những vấn đề cơ bản tại cảng biển như giám sát tải trọng, quản lý ra vào cảng, bãi đỗ xe, và mực nước khai thác. Kết quả đạt được đã cho thấy tiềm năng của việc ứng dụng công nghệ IoT trong việc tối ưu hóa hoạt động cảng biển.

- Khả năng ứng dụng thực tiễn: Dự án đã mô phỏng thành công các chức năng chính của một hệ thống quản lý cảng thông minh, chứng minh khả năng cải thiện hiệu suất vận hành và an toàn.

- Tối ưu hóa qua công nghệ: Việc sử dụng ESP32, giao thức MQTT và các cảm biến đã giúp tích hợp các thành phần điện tử vào một hệ thống thống nhất, hoạt động ổn định và tương thích tốt.

- Cơ sở cho nghiên cứu tương lai: Mô phỏng này là bước đầu tiên để xây dựng và thử nghiệm các tính năng, từ đó tạo cơ sở triển khai hệ thống ở quy mô thực tế với độ phức tạp và tính năng nâng cao hơn.

Tuy đây chỉ là mô phỏng, nhưng dự án đã cung cấp cái nhìn toàn diện về cách ứng dụng IoT để hiện đại hóa hoạt động tại cảng biển. Hệ thống là tiền đề để phát triển thêm các chức năng và thử nghiệm thực tế, đóng góp tích cực vào việc đổi mới công nghệ trong lĩnh vực hàng hải và logistics.
