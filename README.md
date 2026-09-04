# Hệ thống tưới tiêu thông minh

Hệ thống tưới cây tự động thông minh sử dụng vi điều khiển ESP8266 kết hợp cảm biến, màn hình hiển thị, ứng dụng quản lý từ xa qua Blynk, lưu trữ dữ liệu tập trung trên Google Sheets và tự động tối ưu lượng nước tưới dựa theo dự báo thời tiết từ AccuWeather.

---

## 📌 Tính Năng Nổi Bật

- **Chế độ hoạt động linh hoạt (Tự động / Thủ công):** 
  - Chuyển đổi dễ dàng giữa chế độ tự động và thủ công bằng nút nhấn cứng hoặc qua ứng dụng Blynk.
  - Ở chế độ thủ công: Cho phép đóng/ngắt máy bơm trực tiếp bằng nút nhấn hoặc công tắc trên Blynk.
- **Tối ưu hóa nước tưới theo Thời tiết (AccuWeather API):**
  - Tự động truy vấn dữ liệu dự báo thời tiết theo ngày/đêm.
  - Tự động nhận biết khả năng có mưa và phân loại mức độ mưa (**Mưa nhỏ**, **Mưa vừa**, **Mưa lớn**).
  - Tự động điều chỉnh ngưỡng độ ẩm ngắt bơm thông minh: Giảm lượng nước tưới khi trời sắp mưa để tiết kiệm điện/nước và tránh úng cây.
- **Giám sát & Điều khiển từ xa (Blynk IoT):**
  - Cập nhật liên tục Nhiệt độ, Độ ẩm không khí và Độ ẩm đất lên ứng dụng Blynk theo thời gian thực.
  - Đồng bộ trạng thái máy bơm và chế độ hoạt động giữa thiết bị và ứng dụng.
- **Ghi nhật ký dữ liệu Đám mây (Google Sheets):**
  - Tự động gửi dữ liệu cảm biến định kỳ (mỗi 60 giây) lên Google Sheets 
- **Màn hình hiển thị tại chỗ (LCD 16x2 I2C):**
  - Hiển thị trực quan thông số `T` (Nhiệt độ), `H` (Độ ẩm không khí), `SWC` (Độ ẩm đất), trạng thái máy bơm (`ON`/`OFF`) và tình hình thời tiết hiện tại.


---

## 🛠 Phần Cứng Sử Dụng

| Tên Linh Kiện | Số Lượng | Mô Tả / Chức Năng |
| :--- | :---: | :--- |
| **NodeMCU ESP8266** | 1 | Bo mạch điều khiển trung tâm tích hợp Wi-Fi |
| **Cảm biến DHT11** | 1 | Đo nhiệt độ và độ ẩm không khí |
| **Cảm biến Độ ẩm đất (Analog)** | 1 | Đo độ ẩm đất (chân A0) |
| **Màn hình LCD 16x2 + Module I2C** | 1 | Hiển thị thông số hoạt động trực tiếp |
| **Module Relay 5V** | 1 | Điều khiển bật/tắt máy bơm nước |
| **Nút nhấn (Push button)** | 2 | Nút chuyển chế độ & Nút bật/tắt bơm thủ công |
| **Máy bơm nước mini 5V/12V** | 1 | Thiết bị chấp hành tưới nước |

---

## 🔌 Sơ Đồ Kết Nối Chân (Pinout)

| Linh Kiện | Chân Linh Kiện | Chân ESP8266 | Chức Năng / Ghi Chú |
| :--- | :---: | :---: | :--- |
| **Relay Máy Bơm** | IN | `D7` | Đóng/cắt nguồn máy bơm (`OUTPUT`) |
| **Nút Chuyển Chế Độ** | PIN | `D3` | Chuyển Tự động / Thủ công (`INPUT_PULLUP`) |
| **Nút Bật/Tắt Bơm** | PIN | `D1` | Điều khiển bơm thủ công (`INPUT_PULLUP`) |
| **Cảm biến DHT11** | DATA | `D2` | Đọc dữ liệu nhiệt độ & độ ẩm |
| **Cảm biến Độ ẩm đất** | AOUT | `A0` | Đọc tín hiệu độ ẩm đất (Analog 0-1023) |
| **LCD 16x2 I2C** | SDA | `D5` | Chân dữ liệu I2C |
| **LCD 16x2 I2C** | SCL | `D6` | Chân xung giữ nhịp I2C |

---

## 🌐 Cấu Hình Blynk (Virtual Pins)

* **`V0`**: Nhiệt độ môi trường (°C)
* **`V1`**: Độ ẩm không khí (%)
* **`V2`**: Độ ẩm đất SWC (%)
* **`V3`**: Trạng thái Máy Bơm (0: Tat, 1: Bat)
* **`V4`**: Chế độ Hoạt động (0: Thu cong, 1: Tu dong)
* **`V5`**: Trạng thái Dự báo mưa (0: Khong mua, 1: Co mua)

---

## ⚙️ Bảng Logic Tưới Tự Động

Trong **Chế độ Tự động**, máy bơm sẽ tự động bật khi **Độ ẩm đất < 10%**. Ngưỡng độ ẩm để **Tắt máy bơm** thay đổi linh hoạt theo dự báo thời tiết:

| Tình Hình Thời Tiết | Ngưỡng Bật Bơm | Ngưỡng Tắt Bơm | Ghi Chú Tối Ưu |
| :--- | :---: | :---: | :--- |
| **Không Mưa** | `< 10%` | **`> 50%`** | Tưới no nước tiêu chuẩn |
| **Mưa Nhỏ (Level 1)** | `< 10%` | **`> 35%`** | Giảm tưới, chờ nước mưa bổ sung |
| **Mưa Vừa (Level 2)** | `< 10%` | **`> 25%`** | Tưới nhẹ |
| **Mưa Lớn (Level 3)** | `< 10%` | **`> 20%`** | Tưới tối thiểu để tránh ngập úng |

---

## 📚 Thư Viện Cần Thiết (Arduino IDE)

Trước khi biên dịch code, bạn cần cài đặt các thư viện sau trong Arduino IDE (**Tools -> Manage Libraries...**):

1. **`ESP8266WiFi`** & **`ESP8266HTTPClient`** (Sẵn có trong ESP8266 Core)
2. **`Blynk`** (bởi Volodymyr Shymanskyy)
3. **`ArduinoJson`** (phiên bản 6.x)
4. **`DHT sensor library`** (bởi Adafruit)
5. **`LiquidCrystal_I2C`** (bởi Frank de Brabander)

---

