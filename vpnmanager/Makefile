include $(TOPDIR)/rules.mk

PKG_NAME:=vpnmanager
PKG_VERSION:=1.0
PKG_RELEASE:=1
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/vpnmanager
	DEPENDS:=+libubus +libubox +libblobmsg-json
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=Example program that interacts with ubus process
endef

define Package/vpnmanager/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/vpnmanager $(1)/usr/bin/vpnmanager
	echo "this = $(PKG_BUILD_DIR)"
endef

$(eval $(call BuildPackage,vpnmanager))
