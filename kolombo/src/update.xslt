<xsl:stylesheet version = '1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>

<xsl:template match="/">
<html>

	<head>
	</head>
	<body>
	<table border="0" cellpadding="0" cellspacing="0" width="100%">
		<tr>
			<td colspan="2">
			<xsl:attribute name="style">
			<xsl:text>background-color:#3991EE; color : white;height: 1.7em</xsl:text>
			</xsl:attribute>
			</td>
		</tr>
		<tr id="logo">
			<td valign="top">
				<img src="/usr/share/doc/kde/HTML/es/common/kde_logo.png" alt="KDE-The K Desktop Environment" width="296" height="79" border="0"/>
			</td>
			<td valign="middle" align="center" id="location">
				<h1>Actualizaciones disponibles</h1>
			</td>
		</tr>
		<tr>
			<td colspan="2" align="right">
			<xsl:attribute name="style">
			<xsl:text>background-color:#3991EE; color:white;height: 1.7em</xsl:text>
			</xsl:attribute>
			<B>Kolombo</B>
			</td>
		</tr>
	</table>
     <xsl:apply-templates/>
	</body>
</html>
</xsl:template>

<xsl:template match="update">
	<xsl:apply-templates/>
</xsl:template>

<xsl:template match="version">
	<table border = "1">
		<tr width="100%">
			<td width="100%">
				<p align="center">
				<H1>
					<xsl:text>Version: </xsl:text><xsl:value-of select="@number"/>
				</H1>
				</p>
				<xsl:apply-templates/>
			</td>
		</tr>
	</table>
</xsl:template>

<xsl:template match="url">
	<B><xsl:value-of select="@type"/><xsl:text>: </xsl:text></B>
	<a>
		<xsl:attribute name="href">
		<xsl:value-of select="."/>
		</xsl:attribute>
		<xsl:value-of select="."/>
	</a>
	<br/>
</xsl:template>

<xsl:template match="changelog">
	<H1><xsl:text>Changelog: </xsl:text></H1>
		<xsl:apply-templates/>
</xsl:template>

<xsl:template match="optimizations">
	<H2><xsl:text>Optimizaciones: </xsl:text></H2>
	<ul>
		<xsl:for-each select="item">
			<li><xsl:value-of select="."/> </li>
		</xsl:for-each>
	</ul>
</xsl:template>

<xsl:template match="fixes">
	<H2><xsl:text>Correcciones: </xsl:text></H2>
	<ul>
		<xsl:for-each select="item">
			<li>
				<font>
					<xsl:attribute name="style">
					<xsl:choose>
						<xsl:when test="@level=1">
							<xsl:text>background-color:#FF0000</xsl:text>
						</xsl:when>
						<xsl:when test="@level=2">
							<xsl:text>background-color:#FF6161</xsl:text>
						</xsl:when>
						<xsl:when test="@level=3">
							<xsl:text>background-color:#FF8B8B</xsl:text>
						</xsl:when>
						<xsl:when test="@level=4">
							<xsl:text>background-color:#FF8B8B</xsl:text>
						</xsl:when>
						<xsl:when test="@level=5">
							<xsl:text>background-color:#FFCFC9</xsl:text>
						</xsl:when>
						<xsl:otherwise>
							<xsl:text>background-color:white</xsl:text>
						</xsl:otherwise>
					</xsl:choose>
					</xsl:attribute>
					<xsl:value-of select="."/>
				</font>
			</li>
		</xsl:for-each>
	</ul>
</xsl:template>

<xsl:template match="features">
	<H2><xsl:text>Nuevas caracteristicas: </xsl:text></H2>
	<ul>
		<xsl:for-each select="item">
			<li><xsl:value-of select="."/> </li>
		</xsl:for-each>
	</ul>
</xsl:template>

<xsl:template match="todo">
	<H1><xsl:text>Cosas por hacer: </xsl:text></H1>
	<ul>
		<xsl:for-each select="item">
			<li><xsl:value-of select="."/> </li>
		</xsl:for-each>
	</ul>
</xsl:template>

</xsl:stylesheet>
