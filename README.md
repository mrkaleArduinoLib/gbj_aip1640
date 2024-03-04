# gbj_aip1640

Library for utilizing 8 x 16 blue <abbr title='Light Emmiting Diode'>LED</abbr>  matrix display modules WMA451 with AiP1640 controller.

* The controller's communication protocol is dubbed as <abbr title='Inter-Integrated Circuit'>I2C</abbr>, but in fact it is a dedicated simple serial protocol utilizing serial clock and data bus. It is not official I2C protocol due to lack of a bus address and acknowledgment.
* The serial protocol is one-way communication protocol without any feedback from the controller to an <abbr title='Micro Controller Unit'>MCU</abbr> for garanted transmission.
* The library should be considered as a hardware driver for the controller, because it offers methods for utilizing only direcly implemented features of the controller, i.e., sending a byte or a stream of bytes to the controller. Displaying various patterns, symbols, fonts, etc., and manipulation with them on the matrix display is a matter of appropriate application or project libraries.


<a id="Constants"></a>

## Constants
The library does not utilized any error codes, so that it has no constants for them. On the other hand it has constants for geometry of the matrix.

* **Geometry::ROWS**: Number of rows on the matrix. It determines the number of LEDs in a matrix's column. A column can be represented by a single data byte.
* **Geometry::COLUMNS**: Number of columns on the matrix. It determines the number of data bytes in a data byte stream.


### Referencing constants
In a sketch the constants can be referenced in following forms:
* **Static constant** in the form `gbj_aip1640::<enumeration>::<constant>` or shortly `gbj_aip1640::<constant>`, e.g., _gbj_aip1640::Geometry::ROWS_ or _gbj_aip1640::COLUMNS_.
* **Instance constant** in the form `<object>.<constant>`, e.g., _disp.COLUMNS_.


<a id="interface"></a>

## Interface

* [gbj_aip1640()](#constructor)
* [begin()](#begin)
* [display()](#display)
* [displayOff()](#displaySwitch)
* [displayOn()](#displaySwitch)
* [displayToggle()](#displaySwitch)
* [displayFill()](#displayFill)
* [displayClear()](#displayClear)

#### Setters
* [setContrast()](#setContrast)
* [setContrastMin()](#setContrast)
* [setContrastMax()](#setContrast)

#### Getters
* [getContrast()](#getContrast)
* [getContrastMin()](#getContrast)
* [getContrastMax()](#getContrast)
* [isDisplayOn()](#isState)
* [isDisplayOff()](#isState)


<a id="constructor"></a>

## gbj_aip1640()

#### Description
The constructor method stores physical features of the display for the sake of a sketch to the class instance object.

#### Syntax
    gbj_aip1640(uint8_t pinSclk, uint8_t pinDin)

#### Parameters
* **pinSclk**: Microcontroller pin's number utilized as a serial clock of the controller.
	* *Valid values*: non-negative integer (according to a microcontroller datasheet)
	* *Default value*: 2


* **pinDin**: Microcontroller pin's number utilized as a data input of the controller.
	* *Valid values*: non-negative integer (according to a microcontroller datasheet)
	* *Default value*: 3


#### Returns
The library instance object for display processing.

[Back to interface](#interface)


<a id="begin"></a>

## begin()

#### Description
The method sets the pins for the controller, clears the matrix and sets the default contrast of the display.

#### Syntax
	void begin()

#### Parameters
None

#### Returns
None

[Back to interface](#interface)


<a id="display"></a>

## display()

#### Description
The method is overloaded and sends either single byte or a byte stream to the controller.
* The new content of the matrix stays unchanged until another data transmission.
* The method utilizes automatic addressing mode of the driver for byte stream transmission and fixed addressing for single byte transmission.

#### Syntax
	void display(uint8_t dataByte, uint8_t address)
	void display(uint8_t *dataBuffer, uint8_t dataBytes, uint8_t address)

#### Parameters
* **dataByte**: Single byte with the display pattern for particular column.
	* *Valid values*: 0 ~ 255
	* *Default value*: None

* **dataBuffer**: Pointer to a byte array with display patterns for subsequent columns.
	* *Valid values*: microcontroller's addressing range
	* *Default value*: None

* **dataBytes**: Number of bytes to be sent to the controller from the byte array. It determines the number of columns to be updated in the matrix. That number is limited to the remaining count of columns to the end of the matrix from the starting column defined by the address parameter.
	* *Valid values*: 0 ~ [Geometry::COLUMS](#constants)
	* *Default value*: 0

* **address**: Number of a column where a single byte should be displayed of where the byte array should start.
	* *Valid values*: 0 ~ [Geometry::COLUMS](#constants)
	* *Default value*: 0

#### Returns
None

#### See also
[displayOn(), displayOn(), displayToggle()](#displaySwitch)

[Back to interface](#interface)


<a id="displaySwitch"></a>

## displayOn(), displayOff(), displayToggle()

#### Description
Corresponding method either turns on or off the entire display module or toggles its state without changing current contrast level.
* All methods are suitable for making a display module blinking.

#### Syntax
	void displayOn()
	void displayOff()
	void displayToggle()

#### Parameters
None

#### Returns
None

#### See also
[display()](#display)

[isDisplayOn(), isDisplayOff()](#isState)

[Back to interface](#interface)


<a id="displayFill"></a>

## displayFill()

#### Description
The method fills the entire matrix with provided byte, i.e., column pattern.

#### Syntax
	void displayFill(uint8_t dataByte)

#### Parameters
* **dataByte**: Single byte with display pattern for all columns.
	* *Valid values*: 0 ~ 255
	* *Default value*: None

#### Returns
None

#### See also
[displayClear()](#displayClear)

[Back to interface](#interface)


<a id="displayClear"></a>

## displayClear()

#### Description
The method clears the entire matrix.
* It is a wrapper method for method [displayFill()](#displayFill) with zero data byte.

#### Syntax
	void displayClear()

#### Parameters
None

#### Returns
None

#### See also
[displayFill()](#displayFill)

[Back to interface](#interface)


<a id="setContrast"></a>

## setContrast(), setContrastMin(), setContrastMax()

#### Description
The corresponding method sets the respective level of the display contrast.
* The contrast is perceived as the brightness of the display.
* The methods for extreme contrasts are just for convenience.

#### Syntax
	void setContrast(uint8_t contrast)
	void setContrastMin()
	void setContrastMax()

#### Parameters
* **contrast**: Level of contrast/brightness.
	* *Valid values*: 0 ~ 7 ([getContrastMax()](#getContrast))
	* *Default value*: 3

#### Returns
None

#### See also
[getContrast()](#getContrast)

[Back to interface](#interface)


<a id="getContrast"></a>

## getContrast(), getContrastMin(), getContrastMax()

#### Description
The corresponding method returns the respective level of the display contrast stored in the library instance object.
* Methods for extreme contrasts are static.

#### Syntax
	uint8_t getContrast()
	uint8_t getContrastMin()
	uint8_t getContrastMax()

#### Parameters
None

#### Returns
Current or extreme contrast level.

#### See also
[setContrast()](#setContrast)

[Back to interface](#interface)


<a id="isState"></a>

## isDisplayOn(), isDisplayOff()

#### Description
Corresponding method returns a logical flag about current state of the display.

#### Syntax
    bool isDisplayOn()
    bool isDisplayOff()

#### Parameters
None

#### Returns
Flag about display state.

#### See also
[displayOn(), displayOff(), displayToggle()](#displaySwitch)

[Back to interface](#interface)
