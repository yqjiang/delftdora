<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:tud="http://www.tudelft.nl/ns/repo3/">
	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes" media-type="text/xml"/>

	<xsl:template match="/*">
		<xsl:element name="tud:{local-name()}">
			<xsl:for-each select="tud:name"><xsl:call-template name="el"/></xsl:for-each>
			<xsl:for-each select="tud:altName"><xsl:call-template name="el"/></xsl:for-each>
			<xsl:for-each select="tud:birth"><xsl:element name="tud:{local-name()}"><xsl:call-template name="birthDeath"/></xsl:element></xsl:for-each>
			<xsl:for-each select="tud:death"><xsl:element name="tud:{local-name()}"><xsl:call-template name="birthDeath"/></xsl:element></xsl:for-each>
			<xsl:for-each select="tud:education"><xsl:call-template name="el"><xsl:with-param name="prefix" select="'po:'"/></xsl:call-template></xsl:for-each>
			<xsl:for-each select="tud:work"><xsl:call-template name="el"><xsl:with-param name="prefix" select="'po:'"/></xsl:call-template></xsl:for-each>
			<xsl:for-each select="tud:link">
				<xsl:call-template name="el">
					<xsl:with-param name="prefix" select="'http://'"/>
					<xsl:with-param name="lax" select="'http'"/>
				</xsl:call-template>
			</xsl:for-each>
			<xsl:for-each select="tud:memberOf[not(@group='')]"><tud:memberOf group="{@group}"/></xsl:for-each>
			<xsl:for-each select="tud:location"><xsl:call-template name="el"><xsl:with-param name="prefix" select="'gn:'"/></xsl:call-template></xsl:for-each>
			<xsl:for-each select="tud:concept"><xsl:call-template name="el"><xsl:with-param name="prefix" select="'aat:'"/></xsl:call-template></xsl:for-each>
			<xsl:for-each select="tud:description"><xsl:call-template name="el"/></xsl:for-each>
		</xsl:element>
	</xsl:template>

	<xsl:template name="el">
		<xsl:param name="prefix"/>
		<xsl:param name="lax"/>
		<xsl:variable name="txt" select="normalize-space(.)"/>
		<xsl:if test="not($txt=$prefix or $txt='')">
			<xsl:element name="tud:{local-name()}">
				<xsl:copy-of select="@*"/>
				<xsl:choose>
					<xsl:when test="$prefix=''"/>
					<xsl:when test="$lax=''"><xsl:if test="not(starts-with($txt,$prefix))"><xsl:value-of select="$prefix"/></xsl:if></xsl:when>
					<xsl:otherwise><xsl:if test="not(starts-with($txt,$lax))"><xsl:value-of select="$prefix"/></xsl:if></xsl:otherwise>
				</xsl:choose>
				<xsl:value-of select="$txt"/>
			</xsl:element>
		</xsl:if>
	</xsl:template>

	<xsl:template name="birthDeath">
		<xsl:element name="tud:{local-name()}">
			<xsl:for-each select="tud:date"><xsl:call-template name="el"/></xsl:for-each>
			<xsl:for-each select="tud:place"><xsl:call-template name="el"><xsl:with-param name="prefix" select="'gn:'"/></xsl:call-template></xsl:for-each>
		</xsl:element>
	</xsl:template>

</xsl:stylesheet>
